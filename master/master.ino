#include <SoftwareSerial.h>
#include <SPI.h>
#include <Wire.h> 
#include <LiquidCrystal_I2C.h>
#include "Toggle.h"
LiquidCrystal_I2C lcd(0x3F, 20, 4);

SoftwareSerial mySerial(5, 6);
ToggleSwitch em_off(A3);
ToggleSwitch c_f_type(A2);
ToggleSwitch c_d_type(A1);
ToggleSwitch c_f(A0);

int trans_type = 0; // 1:f1, 2:f2, 3:fd;
const int Spins[2] = {10, 9};
int f1_get_idx = 0;

bool e_fd_flag = 0;
byte emergency = 0;

int f1_control_type = 0;
int f1_states[7]; // 0: temp, 1:humi, 3:humi, 2:fan, 3:led;
byte f1_rCommand[7] = {'t', 'h', 'c', 'f', 'l', 'H', 'v'};
byte f1_cCommand[2] = {'F', 'L'};
String f1_tag[7] = {"temp", "humi", "light", "fan", "led", "human", "vib"};

int fd_state = 0;

void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600); 
  mySerial.begin(9600);
  pinMode(Spins[0], OUTPUT);  
  pinMode(Spins[1], OUTPUT); 
  digitalWrite(Spins[0], HIGH);   
  digitalWrite(Spins[1], HIGH); 
  SPI.begin(); 
  SPI.setClockDivider(SPI_CLOCK_DIV16); 

  lcd.begin();
  lcd.backlight();
  lcd.clear();
}

void controlFD(){
  if(fd_state == 1){
    mySerial.write('q');
    Serial.write("q");
  }
  else if(fd_state == 0){
    mySerial.write('w');
    Serial.write("w");
  }
}

byte transferAndWait(const byte what){
  byte a = SPI.transfer(what); 
  delayMicroseconds(20); 
  return a; 
  
}

unsigned long em_print_time = 0;  // 비상 상황시 무엇을 프린트 해줘야 할 지 관련
void loop() {
  // put your main code here, to run repeatedly:
  //em_off.play();
  c_f_type.play();
  c_d_type.play();
  c_f.play();

  if(c_f_type.getFlag() == 1){  // trans_type (f1 -> f2 -> fd)
    Serial.println("trans type changed");
    trans_type++;
    if(trans_type >= 4)
      trans_type = 0;
    c_f_type.setFlag(0);
  }

  if(c_d_type.getFlag() == 1){  // data type change 
    Serial.println("change device type");
    if(trans_type == 1){
      f1_control_type = !f1_control_type;
      //Serial.print("f1_control_type: 0 fan / 1 led:   ");
      //Serial.println(f1_control_type);
    }
    else if(trans_type == 3){
      fd_state = !fd_state; // door open <--> door close
    }
    c_d_type.setFlag(0);   
  }

  if(c_f.getFlag() == 1){ // control transfer
    if( trans_type == 1){
      controlF1(f1_control_type);
    }
    else if(trans_type == 3){
      controlFD();
    }
    c_f.setFlag(0);
  }

  if(emergency == 1){ // emergency state
    if(e_fd_flag == 0){ // front door open
      fd_state = 1;
      controlFD();
      e_fd_flag = true; 
     }else ;
     
   emergencyDataProc();
   if(millis() - em_print_time > 1000){
      if(f1_states[5] > 0){
        lcd.clear();
        
        lcd.setCursor(0, 0);
        lcd.print("floor2:");
        lcd.setCursor(0, 2);
        lcd.print("human: ");
        lcd.print(f1_states[5]);
      }
      else{
        lcd.clear();
        lcd.setCursor(0, 0);
        lcd.print("emergency!!!!!!");
      }
      em_print_time = millis();
   }  
  }else{  // 비상 상황이 아님
    control_room();
  }
}

void printFD(){
  lcd.clear();
  if(fd_state == 1)
    lcd.print("you can open door");
  else if(fd_state == 0)
    lcd.print("you can close door");
  lcd.setCursor(0, 3);
  lcd.print("Front Door");
}

void printF2Data(){
  lcd.setCursor(0, 0);
  lcd.print("detected human: ");
  lcd.print(f1_states[5]);
  lcd.setCursor(0, 3);
  lcd.print("floor2");
}

void printF1Data(){
  for(int i=0; i<3; i++){
    lcd.setCursor(0, i);
    lcd.print(f1_tag[i]);
    lcd.print(": ");
    lcd.print(f1_states[i]);
  }
  for(int i=0; i<2; i++){
    lcd.setCursor(11, i);
    lcd.print(f1_tag[3+i]);
    lcd.print(": ");
    lcd.print(f1_states[3+i]);
  } 
  lcd.setCursor(0, 3);
  lcd.print("f1_control: ");
  if(f1_control_type == 0)
    lcd.print("fan");
  else lcd.print("led");
}


unsigned long pre_time2 = 0;
void getF1Data(){
  byte s;
  pre_time2 = millis();
  digitalWrite(Spins[0], LOW);
  transferAndWait(f1_rCommand[f1_get_idx]);
  transferAndWait(0);
  s = transferAndWait(0);
  digitalWrite(Spins[0], HIGH);
  f1_states[f1_get_idx] = (int)s;

  Serial.print(f1_tag[f1_get_idx]);
  Serial.println(f1_states[f1_get_idx]);
   
  f1_get_idx++;
  if(f1_states[1] >= 90 || f1_states[6] == 1) {
      emergency = 1; 
    //controlF2('E');
  }else {
    mySerial.write('e');
  }
}

unsigned long pre_time3 = 0;
int e_get_data_flag = 0;
void emergencyDataProc(){
  byte s;

  if(millis() - pre_time3 > 1500 && e_get_data_flag == 2){
    e_get_data_flag = 0;
    pre_time3 = millis();

    digitalWrite(Spins[0], LOW);
    transferAndWait(f1_rCommand[6]);  // 습도 정보 가져옴
    transferAndWait(0);
    s = transferAndWait(0);
    digitalWrite(Spins[0], HIGH);
    f1_states[6] = (int)s;

    if(f1_states[1] >= 90 || f1_states[6] == 1) { // 90 미만이면
      emergency = 1;
      mySerial.write('E');
    }else {
      e_fd_flag = false;    
      emergency = 0;     
      mySerial.write('e');
      mySerial.write('w');
    }
  }
  else if(millis() - pre_time3 > 1000 && e_get_data_flag == 1){
    e_get_data_flag++;
    
    digitalWrite(Spins[0], LOW);
    transferAndWait(f1_rCommand[1]);  // 습도 정보 가져옴
    transferAndWait(0);
    s = transferAndWait(0);
    digitalWrite(Spins[0], HIGH);
    f1_states[1] = (int)s;

    if(f1_states[1] >= 90 || f1_states[6] == 1) { // 90 미만이면
      emergency = 1;
      mySerial.write('E');
    }else {
      e_fd_flag = false;    
      emergency = 0;     
      mySerial.write('e');
      mySerial.write('w');
    }
  }
  else if(millis() - pre_time3 > 500 && e_get_data_flag == 0){
    e_get_data_flag++;
    digitalWrite(Spins[0], LOW);
    transferAndWait(f1_rCommand[5]);  // 인원 정보
    transferAndWait(0);
    s = transferAndWait(0);
    digitalWrite(Spins[0], HIGH);
    f1_states[5] = (int)s; 
  }
}

void  controlF1(int type){
  Serial.print("i got: ");
  Serial.println(type);
  digitalWrite(Spins[0], LOW);
  SPI.transfer(f1_cCommand[type]); 
  digitalWrite(Spins[0], HIGH);
}

unsigned long pre_time = 0;
void control_room(){
  if(millis() - pre_time > 700){  
    getF1Data();
    if(f1_get_idx >= 7) f1_get_idx = 0; 
      
    pre_time = millis();
    lcd.clear();
    switch(trans_type){
      case 1:
        printF1Data(); 
        break;
      case 2:
        printF2Data();
        break;
      case 3:
        if(emergency == 1) break;
        printFD();
        break;
      default:break;
    }
  }
}

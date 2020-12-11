#include "Toggle.h"
#include<TimerOne.h>
#include "Distance.h"
#include <Servo.h> 
#define BOUNDARY 9
#include <Wire.h> 
#define sv 8

volatile byte c;
volatile byte command = 0; 
volatile byte data = 0; 
int emergencyflag = 0;
int detect = 0; // human num

const int Buzzer_PIN = 2;
const int LedR_PIN = 8;
const int LedG_PIN = 4;
const int Motor_PIN = 7;
const int pinVibM = 3;

byte vibecount = 0;
boolean earthquakeflag = false;
int warning =0;
int peaceful = 0;

bool s1_flag = 0;
bool s2_flag = 0;

ToggleSwitch sw1(A0);
ToggleSwitch sw2(A1);
USonic sonic1(5, 6);
USonic sonic2(9, 10);

int flag = 0;
int detecting[2] = {0, 0};

void Fanon(){
  digitalWrite(Motor_PIN, HIGH);
};
  
void Fanoff(){
  digitalWrite(Motor_PIN, LOW);
};
void SirenON(){
  tone(Buzzer_PIN,392);
}
void SirenOff(){
  noTone(Buzzer_PIN);
}
void Ledon(){
  digitalWrite(LedG_PIN, HIGH);
  }
void Ledoff(){
  digitalWrite(LedG_PIN, LOW);
}

void emergencymodeon(){
  tone(Buzzer_PIN,392);
  digitalWrite(LedR_PIN, HIGH);
}
void emergencymodeoff(){
  noTone(Buzzer_PIN);
  digitalWrite(LedR_PIN, LOW);
}
void setup() {
  Timer1.initialize(1000000);
  Wire.begin(sv); 
  Wire.onRequest(requestEvent);  
  Wire.onReceive(receiveEvent); 
  Serial.begin(9600);
  pinMode(Motor_PIN,OUTPUT);
  pinMode(LedR_PIN,OUTPUT);
  pinMode(LedG_PIN,OUTPUT);
  pinMode(Buzzer_PIN,OUTPUT);
  pinMode(MISO, OUTPUT);
  pinMode(pinVibM, INPUT);
  attachInterrupt(1, vibecountVib, FALLING);
  Timer1.attachInterrupt(timersonicISR);
}

void requestEvent() { 
  Wire.write(data); 
}
void receiveEvent(int howMany) {  
   char c = Wire.read();
   if(c == 'E')
      emergencyflag = 1;
   else if(c == 'e')
     emergencyflag = 0;
   //Serial.print(c); 
}

unsigned long detect_time = 0;
void loop() {
  sw1.play();
  sw2.play();
  if(sw1.getFlag()){
    Serial.println("motor");
    Fanon();
  }
  else Fanoff();
  
  if(sw2.getFlag()){
    Ledon();
  }
  else Ledoff();

  
  sonic1.detectDistance();
  sonic2.detectDistance();
  
  if(emergencyflag==1){
    emergencymodeon();
  }else emergencymodeoff();

  
  if(sonic1.getDistance() > 5 && sonic1.getDistance() < BOUNDARY ){
    if(detecting[1]== 0 && detecting[0] == 0){  // 처음 임 (들어오는 중)
      Serial.print("sonic1: ");
      Serial.println(sonic1.getDistance());
      detecting[0] = 1;
      //s1_flag = 1;
    }
    else ;
  }else if(sonic2.getDistance() > 5 &&sonic2.getDistance() < BOUNDARY ){
    if(detecting[1]== 0 && detecting[0] == 0){  // 나가는 중
      Serial.print("sonic2: ");
      Serial.println(sonic2.getDistance());
      detecting[1] = 1;
      //s2_flag = 1;
    }
    else ;
  }
  else ;
  
}

void timersonicISR(){
    if(detecting[0] == 1 && detecting[1] == 0){
      detect--;
    }
    else if(detecting[0] == 0 && detecting[1] == 1){
      detect++;
    }
    else ;
    
    s1_flag = 0;
    s2_flag = 0;
    
    data = (byte)detect;
    detecting[0] = 0;
    detecting[1] = 0;
    
    Serial.println(detect);
    //여기 위는 재실감지 관련
    //===================================================
    // 밑은 진동감지
    if (vibecount != 0) {
    vibecount = 0;
    if(warning >= 10)
    earthquakeflag = true;
    if(warning > 50){
    warning = 10;
  }
    warning++;
    peaceful = 0;
} else {
  if(peaceful >= 5)
  earthquakeflag = false;
  if(peaceful > 50){
    peaceful = 0;
}
  warning = 0;
  peaceful++;
 }
 if(earthquakeflag == true){
    //경고 보냄 
  }
}
 void vibecountVib() {
  vibecount++;
}

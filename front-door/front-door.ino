  #include "Doorlock.h"
#include <SoftwareSerial.h>
#include <Servo.h>
#include <LiquidCrystal.h>
Servo servo;
int val;
byte d;
bool emergency_flag = 0;
bool user_control = 0;

int door_flag = 0;  // 0:닫힘., 1:열림

int idx = 0;
int serial_idx = 0;
char c;
SoftwareSerial mySerial(5, 6);
const int pinServo = 10;
const int pinLEDR = 13;
const int pinLEDG = 12;
const int pinLEDB = 11;
const int RS = 2; const int EN = 3;
const int D4 = 4; const int D5 = 7;
const int D6 = 8; const int D7 = 9;
LiquidCrystal lcd1(RS,EN,D4,D5,D6,D7);

int pin[5] = {A5, A0, A1, A2, A3}; // 핀 설정
int password[4] = {1, 2, 3, 1}; // 비밀번호 설정
DoorLock dl(pin); // DoorLock 생성자 생성

void setup() {
  dl.setPassword(password); // 비밀번호 설정(함수)
  Serial.begin(9600); 
  lcd1.begin(16, 2);
  lcd1.print("Hello, Arduino!");
  lcd1.setCursor(0, 1);
  lcd1.print("Embedded Systems");
  delay(1000); lcd1.clear();
  pinMode(pinLEDR, OUTPUT);
  pinMode(pinLEDG, OUTPUT);
  pinMode(pinLEDB, OUTPUT);
  servo.attach(pinServo);
  servo.write(171);
  mySerial.begin(9600);
}

unsigned long pre_time2 = 0;
unsigned long pre_time = 0;
void loop() {
   dl.play();
   
   if(dl.num == 1) {
    digitalWrite(pinLEDR, HIGH);
    digitalWrite(pinLEDG, HIGH);
    digitalWrite(pinLEDB, LOW);
   }
   else if(dl.num == 0) {
      if(dl.getResult() == true){ // 만약 비밀번호가 일치한다면 
        door_flag = 1;
        //servo.write(60);
        digitalWrite(pinLEDB, HIGH);
        digitalWrite(pinLEDR, LOW);
        digitalWrite(pinLEDG, LOW);             
        if(idx == 0){
          pre_time = millis();
          Serial.println("open");  // open을 출력
          idx++;
        }
        else if(idx >= 5){
          dl.setResult(false);
          door_flag = 0;
          idx = 0;
        }
        else ;
        if(millis() - pre_time > 1000){
          pre_time = millis();
          idx++;
        }
        else ;
    }
    else{
      //servo.write(150);
      digitalWrite(pinLEDR, HIGH);
      digitalWrite(pinLEDG, LOW);
      digitalWrite(pinLEDB, LOW);
      door_flag = 0;
    }
   }

   if (mySerial.available()) {
      c = mySerial.read();
      Serial.write(c);
      if(c == 'E') {
        emergency_flag = 1;
      }
      else if(c == 'e') {
        emergency_flag = 0;
        door_flag = 0;
      }
      else if(c == 'q')  {
        user_control = 1; //door_flag = 1;
      }
      else if(c == 'w'){
        user_control = 0;
      }
   }
   if(emergency_flag == 1) servo.write(80);
   else if(user_control == 1) servo.write(80);
   else if(door_flag == 1) servo.write(80);
   else if(door_flag == 0 || user_control == 0){
    servo.write(171); 
   }
}

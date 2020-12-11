#include <Arduino.h>
#include <LiquidCrystal.h>
#include "Toggle.h" // 헤더파일 포함
#include "Switch.h" // 헤더파일 포함
#include "Doorlock.h" // 헤더파일 포함

const int RS = 2; const int EN = 3;
const int D4 = 4; const int D5 = 7;
const int D6 = 8; const int D7 = 9;
LiquidCrystal lcd(RS,EN,D4,D5,D6,D7);
int number[5] = {0,};
int n=0;
DoorLock::DoorLock() { // 기본  DoorLock 생성자
	state = false;
	lastState = state;
	result = false;
}

DoorLock::DoorLock(int *pin) {  // 핀 설정하면서 DoorLock 생성자 
	result = false;
	state = false;
	lastState = state;
	for (int i = 0; i < 5; i++)
  	toggle[i].setPin(pin[i]);
  num = 0;
}

void DoorLock::setPassword(int * password){  // 비밀번호 설정
	for (int i = 0; i < 4; i++) {
		this->password[i] = password[i];
	}
}

void DoorLock::setPin(int * pin){ // 토글 스위치 방식으로 핀 설정
	for (int i = 0; i < 5; i++) {
		toggle[i].setPin(pin[i]);
	}
}

bool DoorLock::getCheck(){  // 정답인지 확인
  for (int i = 0; i < 4; i++) {
    if (password[i] != input[i]){
      number[0] = 0;
      number[1] = 0;
      number[2] = 0;
      number[3] = 0;
      number[4] = 0; 
      n=0;
      lcd.clear();
      lcd.setCursor(0, 0);
      lcd.print("Sorry, wrong");
      Serial.println("Sorry, wrong");
      count = -1;
      return false;
    }
  }
  count = -1;
  number[0] = 0;
  number[1] = 0;
  number[2] = 0;
  number[3] = 0;
  number[4] = 0;  
  n=0;  
  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print("door open!");
  Serial.println("door open!");
  num = 0;
  return true;
}

void DoorLock::play() {    // 버튼 동작
	detectFlag(); // 확인 버튼 감지
  if(state == true){
    if(lastState == false){
      lcd.clear();
      lcd.setCursor(0, 0);
      lcd.print("detect");
      Serial.println("detect");
      lastState = true;
      num = 1;
    }
     getInput();
  }
  else{
    if(lastState == true){
      result = getCheck();
      lcd.clear();
      lcd.setCursor(0, 0);
      lcd.print("check");
      Serial.println("check");
      lastState = false;
    }
    else if(lastState == false){
      ;
    }
  }
}
void DoorLock::getInput() {     // 비밀번호 받아오기

	for (int i = 0; i < 4; i++) {
    toggle[i].play();
		if (toggle[i].getFlag() == HIGH && count < 3) {
			input[++count] = (i+1);
     lcd.setCursor(0, 0);
     number[n++] = i+1;
     lcd.print(number[0]);
     lcd.print(" ");
     lcd.print(number[1]);
     lcd.print(" ");
     lcd.print(number[2]);
     lcd.print(" ");
     lcd.print(number[3]);
     Serial.println(i+1);
      toggle[i].setFlag(LOW);
		}
	}
}

void DoorLock::detectFlag(){  // 플래그 감지(플래그는 버튼이 눌렸는지 안눌렸는지 상태)
	toggle[4].play();
	state = toggle[4].getFlag();
}

int DoorLock::getResult(){ // 결과값 받아오기 
	return result;
}
void DoorLock::setResult(int n){ // 결과값 설정
  result = n;
}

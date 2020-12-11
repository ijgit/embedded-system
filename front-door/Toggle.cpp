#include <Arduino.h>
#include "Toggle.h" // 토글스위치 헤더파일 포함

ToggleSwitch::ToggleSwitch(){;} // 기본 생성자(별거 안해요)
ToggleSwitch::ToggleSwitch(int n){ // n을 입력받고 핀설정하는 생성자
  pin = n; flag = 0; state = 0; lastState = 0; button = 0;
  pinMode(n, INPUT_PULLUP);
}
void ToggleSwitch::setPin(int n){ // 핀설정
  pin = n;
  pinMode(n, INPUT_PULLUP);
}
void ToggleSwitch::setFlag(int n){ // 플래그 설정
  flag = n;
}
int ToggleSwitch::getPin(){ // 핀 받아오기
  return pin;
}
bool ToggleSwitch::getFlag(){ // 플래그 받아오기
  return flag;
}
void ToggleSwitch::play(){ // 버튼 동작
  state = digitalRead(pin);
  if(state != lastState){
    input_time = millis();
  }
  if(millis() - input_time > 50){
    if(state != button){
      button = state;
      if(button == HIGH){
        flag = !flag;
      }
    }
  }
  lastState = state;
}

#ifndef DOORLOCK_H
#define DOORLOCK_H
#include <arduino.h>
#include "Switch.h"
#include "Toggle.h"

class DoorLock
{
  int count = -1;         // 비밀번호를 받아올때 쓰이는 인덱스
  ToggleSwitch toggle[5]; // 토글 스위치 (8버튼)

  int result; // 결과값

  bool state;     // 버튼의 상태
  bool lastState; // 버튼의 이전 상태

  int password[4]; // 4자리 비밀번호
  int input[4];    // 4자리 비밀번호 받아올때 쓰이는 배열

public:
  DoorLock();                      // 기본 생성자
  DoorLock(int *pin);              // 핀 설정 생성자
  void setPassword(int *password); // 비밀번호 설정
  void setPin(int *pin);           // 핀 설정

  void play();       // 버튼 동작
  void getInput();   // 비밀번호 입력받기
  bool getCheck();   // 비밀번호가 맞는지 틀린지 판단
  void detectFlag(); // 플래그 감지

  int getResult();       // 결과값 가져오기
  void setResult(int n); // 결과값 설정
  int num;
};
#endif

# embedded-system

2018-12-06 (Embedded system term project)
|팀장|팀원|
|---|----|
|정이수|김남호, 조의연, 성승우|


## master
- floor1, floor2, front-door 와의 통신기능
- 버튼 패널 표시
- 상태 표시 모니터
- led, 부저 등 긴급 상황 알림 액추에이터


## Floor1
- master 과의 통신 (SPI)
- 진동 감지 센서, 온습도 센서
- fan 외에 led, 부저 등 긴급 상황 알림 액추에이터


## Floor2
- Floor1 과의 통신 (I2C)
- 버튼 패널 및 리모트 컨트롤 구현
- 진동 감지, 온습도 센서
- PIR, 초음파 센서 등을 통해 재실 감지
- fan 외에 led, 부저 등 긴급 상황 알림 액추에이터


## Front-Door
- master 과의 통신 (UART)
- 8 버튼 패널 - 비밀번호 
- 상태 표시 모니터
- 서보 모터 외에 led, 부저 등 긴급 상황 알림 액추에이터

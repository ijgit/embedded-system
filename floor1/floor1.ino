#include <dht11.h>
#include<TimerOne.h>
#include <Wire.h>
#define sv 8

volatile int datas[6] = {0, 0, 0, 0, 0, 0};

volatile byte command = 0;
volatile byte data = 0;

const int pinVibM = 3;
byte vibecount = 0;
boolean earthquakeflag = false;
int warning =0;
int peaceful = 0;

const int pinDHT11 = 2;
int ret;
dht11 DHT11;

const int pinBuzzer = 6;

const int pinLEDR = 8;
const int pinLEDG = 4;
const int pinLEDB = 5;

int fanflag = 0;
int ledflag = 0;
int led_R_flag = 0;
int led_B_flag = 0;
int led_G_flag = 0;
int buzzerflag = 0;

const int pinLight = A2;
int val; // 조도값 플래그
int bright; // 실제 조도값

const int pinRelay = 7;
byte c = 0;

void setup() {
  // put your setup code here, to run once:
  Timer1.initialize(1000000);
  Serial.begin(9600);
  Wire.begin();
  pinMode(pinVibM, INPUT);
  pinMode(pinRelay, OUTPUT);
  pinMode(pinLEDR, OUTPUT);
  pinMode(pinLEDG, OUTPUT);
  pinMode(pinLEDB, OUTPUT);
  pinMode(pinBuzzer,OUTPUT);
  pinMode(MISO, OUTPUT);
  attachInterrupt(1, vibecountVib, FALLING);
  Timer1.attachInterrupt(timerISR);
  SPCR |= _BV(SPE);
  SPCR &= ~_BV(MSTR);
  SPCR |= _BV(SPIE);
}

ISR(SPI_STC_vect) {
  byte c = SPDR;
  int flag = 1;
  if (c == 'F') //
    datas[3] = !datas[3];
  else if (c == 'L')
    datas[4] = !datas[4];
  else flag = 0;

  if (flag == 0) {
    switch (command) {
      case 0:
        command = c;
        // SPDR = 0;
        break;
      case 't': // temp 요구
        SPDR = datas[0];
        break;
      case 'h':   // humi  요구
        SPDR = datas[1];
        break;
      case 'f': // fan 요구
        SPDR = datas[3];
        break;
      case 'l':   // led 요구
        SPDR = datas[4];
        break;
      case 'H':
        SPDR = datas[5];
        break;
      case 'c':   // bright 요구
        SPDR = datas[2];
        break;
      case 'v':
        SPDR = earthquakeflag;
        break;
      default:
        break;
    }
  }
}
unsigned long pre_time3 = 0;
unsigned long pre_time2 = 0;
unsigned long pre_time1 = 0;
void loop() {
  if (millis() - pre_time1 > 1000) {
    pre_time1 = millis();
    ret = DHT11.read(pinDHT11);
    bright = analogRead(pinLight);

    if (bright >= 0 && bright < 300) {
      val = 1;
    }
    else if (bright >= 300 && bright < 500) {
      val = 2;
    }
    else if (bright >= 500) {
      val = 3;
    }

     datas[2] = val;
    switch (ret) {
      case DHTLIB_OK: Serial.println("OK"); break;
      case DHTLIB_ERROR_CHECKSUM:
        Serial.println("Checksum error"); break;
      case DHTLIB_ERROR_TIMEOUT:
        Serial.println("Time out error"); break;
      default: Serial.println("Unknown error"); break;
    }
    Serial.print("Humidity (%): ");
    Serial.println((float)DHT11.humidity, 2);
    datas[1] = (int)DHT11.humidity;

    Serial.print("Temperature (oC): ");
    Serial.println((float)DHT11.temperature, 2);
    datas[0] = (int)DHT11.temperature;
    Serial.println(val);
    Serial.println("Wtest: ");
    Serial.println(warning);
    
  }
  else {
    ;
  }
  
  if(val == 1 && ledflag == 1){
    digitalWrite(pinLEDR, HIGH);
    digitalWrite(pinLEDG, HIGH);
    digitalWrite(pinLEDB, HIGH);
  }else{
    digitalWrite(pinLEDR, LOW);
    digitalWrite(pinLEDG, LOW);
    digitalWrite(pinLEDB, LOW);
  } 
  
  if (digitalRead(SS) == HIGH) {
    command = 0;
  }

  if (millis() - pre_time3 > 1000) {
    if ((float)DHT11.humidity >= 90 || earthquakeflag ==1) {
      fanflag = 0;
      led_R_flag = 1;
      led_G_flag = 0;
      buzzerflag = 1;
      Wire.beginTransmission(sv);
      char c = 'E';
      Wire.write(c);
      Wire.endTransmission();
    } else {
      led_R_flag = 0;
      led_G_flag = 0;
      buzzerflag = 0;
      Wire.beginTransmission(sv);
      char c = 'e';
      Wire.write(c);
      Wire.endTransmission();
    }
    pre_time3 = millis();
  }

  if ((float)DHT11.temperature >= 28 && datas[3] == 1) {
    fanflag = 1;
  }
  
  if (val == 1 && datas[4] == 1) {
    ledflag = 1;
  }
  else {
    fanflag = 0;
    ledflag = 0;
  }

  
  if (millis() - pre_time2 > 1000) {
    pre_time2 = millis();
    Wire.requestFrom(sv, 1);
    if (Wire.available()) {
      Serial.println("Sensed data:");
      byte c = Wire.read();
      data = c;
      datas[5] = (int)c;
      Serial.println(c);
    }
  }
  
  if (fanflag == 1) {
    digitalWrite(pinRelay, HIGH);
  } else if (fanflag == 0) {
    digitalWrite(pinRelay, LOW);
  }

  if (led_R_flag == 1) {
    digitalWrite(pinLEDR, HIGH);
  } else if (led_R_flag == 0) {
    digitalWrite(pinLEDR, LOW);
  }

  if (led_B_flag == 1) {
    digitalWrite(pinLEDB, HIGH);
  } else if (led_B_flag == 0) {
    digitalWrite(pinLEDB, LOW);
  }

  if (led_G_flag == 1) {
    digitalWrite(pinLEDG, HIGH);
  } else if (led_G_flag == 0) {
    digitalWrite(pinLEDG, LOW);
  }
  
  if (buzzerflag == 1){
    tone(pinBuzzer,392);
  }else if(buzzerflag == 0){
    noTone(pinBuzzer);
    }
 }

void timerISR(){
  if (vibecount != 0) {
  vibecount = 0;
  if(warning >= 5)
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
}
void vibecountVib() {
  vibecount++;
}

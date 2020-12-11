#ifndef DISTANCE_H_
#define DISTANCE_H_
#include <Arduino.h>

class USonic
{
  int trigPin;
  int echoPin;
  float distance;

public:
  USonic();
  USonic(int tP, int eP);
  void setPin(int tP, int eP);
  void detectDistance();
  float getDistance();
};

#endif /* DISTANCE_H_ */

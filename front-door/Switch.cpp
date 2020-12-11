#include "Switch.h"
#include <Arduino.h>

Switch::Switch() : Switch(3){};
Switch::Switch(int n)
{
  pin = n;
  state = 0;
  lastState = 0;
  button = 0;
  debouncing = 0;
  input_time = 0;
}

void Switch::setPin(int n)
{
  pin = n;
}

bool Switch::detect()
{
  state = digitalRead(pin);
  if (state != lastState)
  {
    input_time = millis();
  }
  if (millis() - input_time > 50)
  {
    if (state != button)
    {
      button = state;
      if (button == HIGH)
      {
        button = LOW;
        return HIGH;
      }
    }
  }
  state = LOW;
  lastState = state;
  return LOW;
}

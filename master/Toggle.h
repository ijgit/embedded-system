#ifndef TOGGLESWITCH
#define TOGGLESWITCH

class ToggleSwitch{
  int pin;
  bool flag;
  bool state, lastState, button;
  unsigned long input_time;
  unsigned long debouncing;
public:
  ToggleSwitch();
  ToggleSwitch(int n);
  int getPin();
  bool getFlag();
  void setPin(int n);
  void setFlag(int n);
  void play();
};

#endif

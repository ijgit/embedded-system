#ifndef SWITCH_H
#define SWITCH_H

class Switch {
	int pin;
	bool state, lastState, button;
	unsigned long input_time;
	unsigned long debouncing;
public:
	Switch();
	Switch(int n);
	void setPin(int n);
	bool detect();
};

#endif

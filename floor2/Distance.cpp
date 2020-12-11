#include "Distance.h"

USonic::USonic(){;}

USonic::USonic(int tP, int eP){
	setPin(tP, eP);
	pinMode(trigPin, OUTPUT);
	pinMode(echoPin, INPUT);
}

void USonic:: setPin(int tP, int eP){
	trigPin = tP;
	echoPin = eP;
}

void USonic::detectDistance(){
	digitalWrite(trigPin, LOW);
	digitalWrite(echoPin, LOW);
	delayMicroseconds(2);
	digitalWrite(trigPin, HIGH);
	delayMicroseconds(10);
	digitalWrite(trigPin, LOW);
	unsigned long duration = pulseIn(echoPin, HIGH);
	distance = duration/29.0/2.0;
}

float USonic::getDistance(){
	return distance;
}

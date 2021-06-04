#include <Arduino.h>

#include "ElevatorController.h"

ElevatorController elevatorController;
void setup() {
	// put your setup code here, to run once:
	Serial.begin(9600);
	elevatorController.init();
}

void loop() {
	// put your main code here, to run repeatedly:
	elevatorController.run();
}

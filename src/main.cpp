#include <Arduino.h>

#include "ElevatorController.h"

ElevatorController elevatorController;
void setup() {
	Serial.begin(9600);
	elevatorController.init();
}

void loop() {
	elevatorController.run();
}

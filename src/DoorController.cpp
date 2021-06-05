#include "Arduino.h"
#include "DoorController.h"


void DoorController::init(int motorPin1, int motorPin2, int motorPin3, int motorPin4, int pingPin, int echoPin) {
    stepperController.init(DM_STEPS_PER_REVOLUTION, DM_RPM, motorPin1, motorPin2, motorPin3, motorPin4);
    this->pingPin = pingPin;
    this->echoPin = echoPin;
    pinMode(pingPin, OUTPUT);
    pinMode(echoPin, INPUT);
}

bool DoorController::open() {
    // Check if the door is already opened
    if (stepperController.getCurrentStep() == OPENED_DOOR_STEP) {
        return true;
    }
    stepperController.step(-1);
    if (stepperController.getCurrentStep() == OPENED_DOOR_STEP) {
        return true;
    } else {
        return false;
    }
}

bool DoorController::close() {
    // Check if the door is already closed
    if (stepperController.getCurrentStep() == CLOSED_DOOR_STEP) {
        return true;
    }
    stepperController.step(1);
    if (stepperController.getCurrentStep() == CLOSED_DOOR_STEP) {
        // Assuming door is closed at step 0
        return true;
    } else {
        return false;
    }
}

long microsecondsToCentimeters(long microseconds) {
    // Speed of sounds = 343 m/s = 0.0343 cm / microseconds = 1 / 29 cm / microseconds
    // Distance (roundtrip) = speed * time = microseconds / 29
    // Distance (one-way) = micrseconds / 29 / 2
    return microseconds / 29 / 2;
}

bool DoorController::checkObstacles() {
    // Send a pulse on the ping (trigger) pin
    digitalWrite(pingPin, LOW);
    delayMicroseconds(2);
    digitalWrite(pingPin, HIGH);
    delayMicroseconds(10);
    digitalWrite(pingPin, LOW);
    // Measure the time for the input pulse on the echo pin
    long duration = pulseIn(echoPin, HIGH);
    long cm = microsecondsToCentimeters(duration);
    return cm < OBSTALCE_DISTANCE_THRESHOLD_CM;
}
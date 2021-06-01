#include "Arduino.h"
#include "StepperController.h"

#define CIRCULAR_INC(X, MAX) X = (X + 1) % MAX
#define CIRCULAR_DEC(X, MAX) X = X == 0 ? MAX - 1 : X - 1; 


void StepperController::init(uint stepsPerRevolution, uint speed, uint pin1, uint pin2, uint pin3, uint pin4) {
    this->pin1 = pin1;
    this->pin2 = pin2;
    this->pin3 = pin3;
    this->pin4 = pin4;
    pinMode(this->pin1, OUTPUT);
    pinMode(this->pin2, OUTPUT);
    pinMode(this->pin3, OUTPUT);
    pinMode(this->pin4, OUTPUT);

    this->currentPosition = 0;
    this->currentStep = 0;
    this->stepsPerRevolution = stepsPerRevolution;
    this->stepDelay = (60L * 1000L / stepsPerRevolution) / speed;
}

void StepperController::motorStep(bool isClockwise) {
    if (isClockwise) {
        CIRCULAR_INC(currentPosition, 4);
        CIRCULAR_INC(currentStep, stepsPerRevolution);
    } else {
        CIRCULAR_DEC(currentPosition, 4);
        CIRCULAR_DEC(currentStep, stepsPerRevolution);
    }
    switch (currentPosition) {
    case 0:
        digitalWrite(pin1, LOW); 
        digitalWrite(pin2, HIGH); 
        digitalWrite(pin3, LOW); 
        digitalWrite(pin4, LOW);
        break;

    case 1:
        digitalWrite(pin1, LOW); 
        digitalWrite(pin2, LOW); 
        digitalWrite(pin3, LOW); 
        digitalWrite(pin4, HIGH);
        break;

    case 2:
        digitalWrite(pin1, HIGH); 
        digitalWrite(pin2, LOW); 
        digitalWrite(pin3, LOW); 
        digitalWrite(pin4, LOW);
        break;

    case 3:
        digitalWrite(pin1, LOW); 
        digitalWrite(pin2, LOW); 
        digitalWrite(pin3, HIGH); 
        digitalWrite(pin4, LOW);
        break;
    }
}

void StepperController::step(int steps) {
    bool isClockwise = steps > 0;
    for(int i = 0; i < abs(steps); i++) {
        motorStep(isClockwise);
        delay(stepDelay);
    }
}

uint StepperController::getCurrentStep() {
    return currentStep;
}
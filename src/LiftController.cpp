#include "LiftController.h"
#include "Arduino.h"

#define CIRCULAR_INC(step) step = (step + 1) % STEPS_PER_REVOLUTION
#define CIRCULAR_DEC(step) step = (step - 1) % STEPS_PER_REVOLUTION

void LiftController::init(int motorPin1, int motorPin2, int motorPin3, int motorPin4) {
    currentStep = 0;
    stepper = new Stepper(STEPS_PER_REVOLUTION, motorPin1, motorPin2, motorPin3, motorPin4);
    stepper->setSpeed(RPM);
}

bool LiftController::moveUp() {
    stepper->step(1);
    CIRCULAR_INC(currentStep);
    if (currentStep == 0) {
        // Completed one revolution (assuming 1 revolution = 1 floor)
        return true;
    } else {
        return false;
    }
}

bool LiftController::moveDown() {
    stepper->step(-1);
    CIRCULAR_DEC(currentStep);
    if (currentStep == 0) {
        // Completed one revolution (assuming 1 revolution = 1 floor)
        return true;
    } else {
        return false;
    }
}
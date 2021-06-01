#include "LiftController.h"
#include "Arduino.h"


void LiftController::init(int motorPin1, int motorPin2, int motorPin3, int motorPin4) {
    stepperController.init(STEPS_PER_REVOLUTION, RPM, motorPin1, motorPin2, motorPin3, motorPin4);
}

bool LiftController::moveUp() {
    stepperController.step(1);
    if (stepperController.getCurrentStep() == 0) {
        // Completed one revolution (assuming 1 revolution = 1 floor)
        return true;
    } else {
        return false;
    }
}

bool LiftController::moveDown() {
    stepperController.step(-1);
    if (stepperController.getCurrentStep() == 0) {
        // Completed one revolution (assuming 1 revolution = 1 floor)
        return true;
    } else {
        return false;
    }
}
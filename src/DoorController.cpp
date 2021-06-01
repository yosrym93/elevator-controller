#include "DoorController.h"


void DoorController::init(int motorPin1, int motorPin2, int motorPin3, int motorPin4) {
    stepperController.init(STEPS_PER_REVOLUTION, RPM, motorPin1, motorPin2, motorPin3, motorPin4);
}

bool DoorController::open() {
    // Check if the door is already opened
    if (stepperController.getCurrentStep() == STEPS_PER_REVOLUTION/2) {
        return true;
    }
    stepperController.step(1);
    if (stepperController.getCurrentStep() == STEPS_PER_REVOLUTION/2) {
        // Assuming door is opened at half a revolution
        return true;
    } else {
        return false;
    }
}

bool DoorController::close() {
    // Check if the door is already closed
    if (stepperController.getCurrentStep() == 0) {
        return true;
    }
    stepperController.step(-1);
    if (stepperController.getCurrentStep() == 0) {
        // Assuming door is closed at step 0
        return true;
    } else {
        return false;
    }
}
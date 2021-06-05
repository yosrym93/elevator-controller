#pragma once
#include "constants.h"
#include "StepperController.h"

class LiftController {
private:
    StepperController stepperController;
public:
    void init(int motorPin1, int motorPin2, int motorPin3, int motorPin4);
    bool moveUp();
    bool moveDown();
};
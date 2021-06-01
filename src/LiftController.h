#pragma once
#include "StepperController.h"

#define STEPS_PER_REVOLUTION 90
#define RPM 12

class LiftController {
private:
    StepperController stepperController;
public:
    void init(int motorPin1, int motorPin2, int motorPin3, int motorPin4);
    bool moveUp();
    bool moveDown();
};
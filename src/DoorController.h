#pragma once
#include "constants.h"
#include "StepperController.h"

class DoorController {
private:
    int pingPin, echoPin;
    StepperController stepperController;
public:
    void init(int motorPin1, int motorPin2, int motorPin3, int motorPin4, int pingPin, int echoPin);
    bool checkObstacles();
    bool open();
    bool close();
};
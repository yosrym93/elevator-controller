#pragma once
#include "StepperController.h"

#define DM_STEPS_PER_REVOLUTION 90
#define DM_RPM 12
#define OBSTALCE_DISTANCE_THRESHOLD_CM 200

#define CLOSED_DOOR_STEP 0
#define OPENED_DOOR_STEP DM_STEPS_PER_REVOLUTION/2

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
#pragma once

#include "constants.h"
#include "DoorController.h"
#include "LiftController.h"
#include "IOController.h"

class ElevatorController {
private:
    enum ElevatorState {
        IDLE,
        GOING_UP,
        GOING_DOWN,
        DOOR_OPENING,
        DOOR_WAITING,
        DOOR_CLOSING
    } elevatorState;

    NextStopPlanningState<NUM_FLOORS> nextStopPlanningState;
    DoorController doorController;
    LiftController liftController;
    IOController ioController;

    uint8_t currentFloor;
    unsigned long doorWaitBeginningMillis;
public:
    void init();
    void run();
};
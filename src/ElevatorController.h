#pragma once

#include "constants.h"
#include "DoorController.h"
#include "LiftController.h"
#include "IOController.h"

class ElevatorController {
private:
    enum ElevatorState {
        IDLE, // The door is opened when the elevator is IDLE
        GOING_UP,
        GOING_DOWN,
        DOOR_OPENING,
        DOOR_WAITING,
        DOOR_CLOSING,
        DOOR_CLOSED_AT_FLOOR,
    } elevatorState;

    NextStopPlanningState<NUM_FLOORS> nextStopPlanningState;
    DoorController doorController;
    LiftController liftController;
    IOController ioController;

    unsigned long doorWaitBeginningMillis;
public:
    void init();
    void run();
};
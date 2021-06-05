#pragma once

#include "constants.h"
#include "DoorController.h"
#include "LiftController.h"
#include "IOController.h"

class ElevatorController {
private:
    enum ElevatorState {
        // The elevator is IDLE at a floor with the door opened at the currentFloor.
        IDLE,
        // The elevator is moving up from the currentFloor.
        GOING_UP,
        // The elevator is moving down from the currentFloor.
        GOING_DOWN,
        // The elevator door is being opened.
        DOOR_OPENING,
        // The elevator door is waiting for people to get in before closing.
        DOOR_WAITING,
        // The elevator door is being closed.
        DOOR_CLOSING,
    } elevatorState;

    NextStopPlanningState<NUM_FLOORS> nextStopPlanningState;
    DoorController doorController;
    LiftController liftController;
    IOController ioController;
    unsigned long doorWaitBeginningMillis;

    void onFloorReached();
public:
    void init();
    void run();
};
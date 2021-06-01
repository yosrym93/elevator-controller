#pragma once
#include "DoorController.h"
#include "LiftController.h"

class ElevatorController {
private:
    enum State {
        IDLE,
        GOING_UP,
        GOING_DOWN,
        DOOR_OPENING,
        DOOR_WAITING,
        DOOR_CLOSING
    };

    State state;
    DoorController doorController;
    LiftController liftController;

    unsigned long doorWaitBeginningMillis;
public:
    void init();
    void run();
};
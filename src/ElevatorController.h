#include "DoorController.h"
#include "LiftController.h"

class ElevatorController {
private:
    enum State {
        IDLE,
        GOING_UP,
        GOING_DOWN,
        DOOR_OPENING,
        DOOR_CLOSING
    };

    State state;
    DoorController doorController;
    LiftController liftController;
public:
    void init();
    void run();
};
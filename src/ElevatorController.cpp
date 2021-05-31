#include "ElevatorController.h"
#include "Arduino.h"

void ElevatorController::init() {
    Serial.println("Initializing elevator controller");

    state = IDLE;
    doorController.init();
    liftController.init();

    // TODO: initialize pins
}

void ElevatorController::run() {
    bool  done;
    switch (state) {
        case IDLE:
            break;
        case GOING_UP:
            done = liftController.moveUp();
            if (done) {
                state = IDLE;
            }
            break;
        case GOING_DOWN:
            done = liftController.moveDown();
            if (done) {
                state = IDLE;
            }
            break;
        case DOOR_OPENING:
            done = doorController.open();
            if (done) {
                state = IDLE;
            }
            break;
        case DOOR_CLOSING:
            done = doorController.close();
            if (done) {
                state = IDLE;
            }
            break;
    } 
}
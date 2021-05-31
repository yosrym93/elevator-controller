#include "ElevatorController.h"
#include "Arduino.h"

void ElevatorController::init() {
    Serial.println("Initializing elevator controller");
    state = IDLE;

    // TODO: initialize pins
}

void ElevatorController::run() {
    switch (state) {
        case IDLE:
            // TODO
            break;
        case GOING_UP:
            // TODO
            break;
        case GOING_DOWN:
            // TODO
            break;
        case DOOR_OPENING:
            // TODO
            break;
        case DOOR_CLOSING:
            // TODO
            break;
    } 
}
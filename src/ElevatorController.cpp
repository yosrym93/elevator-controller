#include "ElevatorController.h"
#include "Arduino.h"

void ElevatorController::init() {
    Serial.println("Initializing elevator controller");

    state = DOOR_OPENING;
    doorController.init(DOOR_MOTOR_PIN1, DOOR_MOTOR_PIN2, DOOR_MOTOR_PIN3, DOOR_MOTOR_PIN4, US_PING_PIN, US_ECHO_PIN);
    liftController.init(LIFT_MOTOR_PIN1, LIFT_MOTOR_PIN2, LIFT_MOTOR_PIN3, LIFT_MOTOR_PIN4);
    ioController.init();
}

void ElevatorController::run() {
    ioController.readFloorsInput();
    ioController.readElevatorNumpad();
    ioController.displayInput();
    bool done;
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
                state = DOOR_WAITING;
                doorWaitBeginningMillis = millis();
            }
            break;
        case DOOR_WAITING:
            if (doorController.checkObstacles()) {
                // Refresh wait time if an obstacle is detected
                doorWaitBeginningMillis = millis();
            }
            else if (millis() - doorWaitBeginningMillis >= DOOR_WAIT_SECONDS * 1000L) {
                state = DOOR_CLOSING;
            }
            break;
        case DOOR_CLOSING:
        if (doorController.checkObstacles()) {
                // Open the door if an obstacle is detected
                state = DOOR_OPENING;
            }
            done = doorController.close();
            if (done) {
                state = IDLE;
            }
            break;
    } 
}
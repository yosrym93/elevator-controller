#include "ElevatorController.h"
#include "Arduino.h"

void ElevatorController::init() {
    Serial.println("Initializing elevator controller");

    currentFloor = 0;
    nextStopPlanningState = ZERO_STRUCT;
    elevatorState = DOOR_OPENING;
    doorController.init(DOOR_MOTOR_PIN1, DOOR_MOTOR_PIN2, DOOR_MOTOR_PIN3, DOOR_MOTOR_PIN4, US_PING_PIN, US_ECHO_PIN);
    liftController.init(LIFT_MOTOR_PIN1, LIFT_MOTOR_PIN2, LIFT_MOTOR_PIN3, LIFT_MOTOR_PIN4);
    ioController.init();
}

void ElevatorController::run() {
    delay(500);
    ioController.readInput();
    ioController.displayInput();
    bool done;
    switch (elevatorState) {
        case IDLE:
            break;
        case GOING_UP:
            done = liftController.moveUp();
            if (done) {
                elevatorState = IDLE;
            }
            break;
        case GOING_DOWN:
            done = liftController.moveDown();
            if (done) {
                elevatorState = IDLE;
            }
            break;
        case DOOR_OPENING:
            done = doorController.open();
            if (done) {
                elevatorState = DOOR_WAITING;
                doorWaitBeginningMillis = millis();
            }
            break;
        case DOOR_WAITING:
            if (doorController.checkObstacles()) {
                // Refresh wait time if an obstacle is detected
                doorWaitBeginningMillis = millis();
            }
            else if (millis() - doorWaitBeginningMillis >= DOOR_WAIT_SECONDS * 1000L) {
                elevatorState = DOOR_CLOSING;
            }
            break;
        case DOOR_CLOSING:
        if (doorController.checkObstacles()) {
                // Open the door if an obstacle is detected
                elevatorState = DOOR_OPENING;
            }
            done = doorController.close();
            if (done) {
                elevatorState = IDLE;
            }
            break;
    } 
}
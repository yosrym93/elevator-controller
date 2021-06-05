#include "ElevatorController.h"
#include "Arduino.h"

void ElevatorController::init() {
    Serial.println("Initializing elevator controller");

    nextStopPlanningState = ZERO_STRUCT;
    elevatorState = IDLE;
    doorController.init(DOOR_MOTOR_PIN1, DOOR_MOTOR_PIN2, DOOR_MOTOR_PIN3, DOOR_MOTOR_PIN4, US_PING_PIN, US_ECHO_PIN);
    liftController.init(LIFT_MOTOR_PIN1, LIFT_MOTOR_PIN2, LIFT_MOTOR_PIN3, LIFT_MOTOR_PIN4);
    ioController.init();
}

void ElevatorController::run() {
    auto input = ioController.readInput();
    nextStopPlanningState = nextStopPlanningState.getNextState(&input);
    bool arrivedAtNextFloor, done;
    switch (elevatorState) {
        case IDLE:
            if (nextStopPlanningState.nextFloor != nextStopPlanningState.currentFloor) {
                elevatorState = DOOR_WAITING;
            }
            break;
        case GOING_UP:
            arrivedAtNextFloor = liftController.moveUp();
            if (arrivedAtNextFloor) {
                nextStopPlanningState.currentFloor++;
                elevatorState = DOOR_CLOSED_AT_FLOOR;
            }
            break;
        case GOING_DOWN:
            arrivedAtNextFloor = liftController.moveDown();
            if (arrivedAtNextFloor) {
                nextStopPlanningState.currentFloor--;
                elevatorState = DOOR_CLOSED_AT_FLOOR;
            }
            break;
        case DOOR_OPENING:
            done = doorController.open();
            if (done) {
                elevatorState = IDLE;
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
                elevatorState = DOOR_CLOSED_AT_FLOOR;
            }
            break;
        case DOOR_CLOSED_AT_FLOOR:
            if (nextStopPlanningState.nextFloor < nextStopPlanningState.currentFloor) {
                elevatorState = GOING_DOWN;
            } else if (nextStopPlanningState.nextFloor > nextStopPlanningState.currentFloor) {
                elevatorState = GOING_UP;
            } else {
                elevatorState = DOOR_OPENING;
            }
            break;
    } 
}
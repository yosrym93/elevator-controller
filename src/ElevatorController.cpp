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
    // Read inputs and update the next stop planning state, which may change our destination.
    delay(500);
    auto input = ioController.readInput();
    nextStopPlanningState = nextStopPlanningState.getNextState(&input);
    ioController.displayInput();
    Serial.print("Current floor: ");
    Serial.print(nextStopPlanningState.currentFloor);
    Serial.print(" , Next floor: ");
    Serial.print(nextStopPlanningState.nextFloor);
    Serial.println("");
    switch (elevatorState) {
        case IDLE: {
            Serial.println("IDLE");
            if (nextStopPlanningState.nextFloor != nextStopPlanningState.currentFloor) {
                // The elevator will start moving, the door should wait for people to get in then close.
                doorWaitBeginningMillis = millis();
                elevatorState = DOOR_WAITING;
            }
            break;
        }
        case GOING_UP: {
            Serial.println("GOING_UP");
            // Keep moving up until the next floor is reached, then move to DOOR_CLOSED_AT_FLOOR state
            // to check whether we need to continue moving up.
            bool arrivedAtNextFloor = liftController.moveUp();
            if (arrivedAtNextFloor) {
                elevatorState = DOOR_CLOSED_AT_FLOOR;
            }
            break;
        }
        case GOING_DOWN: {
            Serial.println("GOING_DOWN");
            // Keep moving down until the next floor is reached, then move to DOOR_CLOSED_AT_FLOOR state
            // to check whether we need to continue moving down.
            bool arrivedAtNextFloor = liftController.moveDown();
            if (arrivedAtNextFloor) {
                elevatorState = DOOR_CLOSED_AT_FLOOR;
            }
            break;
        }
        case DOOR_OPENING: {
            Serial.println("DOOR_OPENING");
            // Keep opening the door until it is fully opened, then move to IDLE state.
            bool done = doorController.open();
            if (done) {
                elevatorState = IDLE;
            }
            break;
        }
        case DOOR_WAITING: {
            Serial.println("DOOR_WAITING");
            // Wait for a timeout for being to get in before closing the door. 
            // Refresh the timeout if obstacles are detected.
            if (doorController.checkObstacles()) {
                // Refresh wait time if an obstacle is detected
                doorWaitBeginningMillis = millis();
            }
            else if (millis() - doorWaitBeginningMillis >= DOOR_WAIT_SECONDS * 1000L) {
                elevatorState = DOOR_CLOSING;
            }
            break;
        }
        case DOOR_CLOSING: {
            Serial.println("DOOR_CLOSING");
        // Keep closing the door until it is fully closing, then move to DOOR_CLOSED_AT_FLOOR state
        // to check whether we should move up or down (or neither if the call is cancelled).
            if (doorController.checkObstacles()) {
                // Open the door if an obstacle is detected.
                elevatorState = DOOR_OPENING;
            }
            bool done = doorController.close();
            if (done) {
                elevatorState = DOOR_CLOSED_AT_FLOOR;
            }
            break;
        }
        case DOOR_CLOSED_AT_FLOOR: {
            Serial.println("DOOR_CLOSED_AT_FLOOR");
            // The elevator arrived at the currentFloor, check whether we need to keep moving
            // or this is our destination.
            if (nextStopPlanningState.nextFloor < nextStopPlanningState.currentFloor) {
                // Need to go further down.
                nextStopPlanningState.isMoving = true;
                nextStopPlanningState.currentFloor--;
                elevatorState = GOING_DOWN;
            } else if (nextStopPlanningState.nextFloor > nextStopPlanningState.currentFloor) {
                // Need to go further up.
                nextStopPlanningState.isMoving = true;
                nextStopPlanningState.currentFloor++;
                elevatorState = GOING_UP;
            } else {
                // Arrived at the destination.
                nextStopPlanningState.isMoving = false;
                elevatorState = DOOR_OPENING;
            }
            break;
        }
    } 
}
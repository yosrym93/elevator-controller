#include "ElevatorController.h"
#include "Arduino.h"

void ElevatorController::init() {
    //Serial.println("Initializing elevator controller");

    nextStopPlanningState = ZERO_STRUCT;
    elevatorState = IDLE;

    pinMode(MOTORS_SELECTOR_PIN, OUTPUT);
    
    doorController.init(MOTOR_PIN1, MOTOR_PIN2, MOTOR_PIN3, MOTOR_PIN4, US_PING_PIN, US_ECHO_PIN);
    liftController.init(MOTOR_PIN1, MOTOR_PIN2, MOTOR_PIN3, MOTOR_PIN4);
    ioController.init();
}

void ElevatorController::enableDoorMotor() {
    digitalWrite(MOTORS_SELECTOR_PIN, DOOR_MOTOR);
}

void ElevatorController::enableLiftMotor(){
    digitalWrite(MOTORS_SELECTOR_PIN, LIFT_MOTOR);
}

void ElevatorController::run() {
    // Read inputs and update the next stop planning state, which may change our destination.
    auto input = ioController.readInput();
    nextStopPlanningState = nextStopPlanningState.getNextState(&input);
    // ioController.displayInput();
    // Serial.print("Current floor: ");
    // Serial.print(nextStopPlanningState.currentFloor);
    // Serial.print(" , Next floor: ");
    // Serial.print(nextStopPlanningState.nextFloor);
    // Serial.println("");
    switch (elevatorState) {
        case IDLE: {
            //Serial.println("IDLE");
            if (nextStopPlanningState.nextFloor != nextStopPlanningState.currentFloor) {
                // The elevator will start moving, the door should wait for people to get in then close.
                doorWaitBeginningMillis = millis();
                elevatorState = DOOR_WAITING;
            }
            break;
        }
        case GOING_UP: {
            //Serial.println("GOING_UP");
            // Keep moving up until the next floor is reached, then move to DOOR_CLOSED_AT_FLOOR state
            // to check whether we need to continue moving up.
            enableLiftMotor();
            bool arrivedAtNextFloor = liftController.moveUp();
            if (arrivedAtNextFloor) {
                updateCurrentFloor(nextStopPlanningState.currentFloor + 1);
                onFloorReached();
            }
            break;
        }
        case GOING_DOWN: {
            //Serial.println("GOING_DOWN");
            // Keep moving down until the next floor is reached, then move to DOOR_CLOSED_AT_FLOOR state
            // to check whether we need to continue moving down.
            enableLiftMotor();
            bool arrivedAtNextFloor = liftController.moveDown();
            if (arrivedAtNextFloor) {
                updateCurrentFloor(nextStopPlanningState.currentFloor - 1);
                onFloorReached();
            }
            break;
        }
        case DOOR_OPENING: {
            //Serial.println("DOOR_OPENING");
            // Keep opening the door until it is fully opened, then move to IDLE state.
            enableDoorMotor();
            bool done = doorController.open();
            if (done) {
                elevatorState = IDLE;
            }
            break;
        }
        case DOOR_WAITING: {
            //Serial.println("DOOR_WAITING");
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
            //Serial.println("DOOR_CLOSING");
            // Keep closing the door until it is fully closing, then move to DOOR_CLOSED_AT_FLOOR state
            // to check whether we should move up or down (or neither if the call is cancelled).
            enableDoorMotor();
            if (doorController.checkObstacles()) {
                // Open the door if an obstacle is detected.
                elevatorState = DOOR_OPENING;
            }
            bool done = doorController.close();
            if (done) {
                // Once the door closes in the currentFloor, the behavior is the same as reaching a floor
                // while moving.
                onFloorReached();
            }
            break;
        }
    } 
}

void ElevatorController::updateCurrentFloor(uint8_t newFloor) {
    nextStopPlanningState.currentFloor = newFloor;
    ioController.output7Segment(nextStopPlanningState.currentFloor);
}

void ElevatorController::onFloorReached() {
    if (nextStopPlanningState.nextFloor < nextStopPlanningState.currentFloor) {
        // Need to go further down.
        nextStopPlanningState.isMoving = true;
        elevatorState = GOING_DOWN;
    } else if (nextStopPlanningState.nextFloor > nextStopPlanningState.currentFloor) {
        // Need to go further up.
        nextStopPlanningState.isMoving = true;
        elevatorState = GOING_UP;
    } else {
        // Arrived at the destination.
        nextStopPlanningState.isMoving = false;
        elevatorState = DOOR_OPENING;
    }
}
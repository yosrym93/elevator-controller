#include "ElevatorController.h"
#include "Arduino.h"

#define DOOR_WAIT_SECONDS 3

#define DOOR_MOTOR_PIN1 6
#define DOOR_MOTOR_PIN2 7
#define DOOR_MOTOR_PIN3 8
#define DOOR_MOTOR_PIN4 9

#define LIFT_MOTOR_PIN1 10
#define LIFT_MOTOR_PIN2 11
#define LIFT_MOTOR_PIN3 12
#define LIFT_MOTOR_PIN4 13

void ElevatorController::init() {
    Serial.println("Initializing elevator controller");

    state = IDLE;
    doorController.init(DOOR_MOTOR_PIN1, DOOR_MOTOR_PIN2, DOOR_MOTOR_PIN3, DOOR_MOTOR_PIN4);
    liftController.init(LIFT_MOTOR_PIN1, LIFT_MOTOR_PIN2, LIFT_MOTOR_PIN3, LIFT_MOTOR_PIN4);
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
                state = DOOR_WAITING;
                doorWaitBeginningMillis = millis();
            }
            break;
        case DOOR_WAITING:
            if (millis() - doorWaitBeginningMillis >= DOOR_WAIT_SECONDS * 1000L) {
                state = DOOR_CLOSING;
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
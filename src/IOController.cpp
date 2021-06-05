#include "IOController.h"
#include "Arduino.h"


void IOController::init() {
    Serial.println("Initializing IO controller");

    init_floors_input();
}

void IOController::init_floors_input() {
    pinMode(FLOORS_REGISTERS_CLOCK, OUTPUT);
    pinMode(FLOORS_REGISTERS_LATCH, OUTPUT);

    pinMode(FLOORS_UP_PIN, INPUT);
    pinMode(FLOORS_DOWN_PIN, INPUT);
    pinMode(FLOORS_IR_PIN, INPUT);
}

void IOController::enable_floors_input() {
    digitalWrite(FLOORS_REGISTERS_LATCH, 1);
}

void IOController::disable_floors_input() {
    digitalWrite(FLOORS_REGISTERS_LATCH, 0);
}

void  IOController::read_floors_input() {
    enable_floors_input();
    delayMicroseconds(20);
    disable_floors_input();

    for (uint8_t i =  - 1; i >= 0; i--) {
        digitalWrite(FLOORS_REGISTERS_CLOCK, 0);
        delayMicroseconds(0.5);

        floorsUpButton[i] = digitalRead(FLOORS_UP_PIN);
        floorsDownButton[i] = digitalRead(FLOORS_DOWN_PIN);
        floorsIRSensor[i] = digitalRead(FLOORS_IR_PIN);

        digitalWrite(FLOORS_REGISTERS_CLOCK, 1);
    }
}

void  IOController::display_floors_input() {
    Serial.print("Up: ");
    for( auto bit : floorsUpButton ) {
        Serial.print(bit);
    }
    Serial.print("\nDown: ");
    for( auto bit : floorsDownButton ) {
        Serial.print(bit);
    }
    Serial.print("\nIR: ");
    for( auto bit : floorsIRSensor ) {
        Serial.print(bit);
    }
    Serial.println(" ");
}
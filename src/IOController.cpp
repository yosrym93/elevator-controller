#include "IOController.h"
#include "Arduino.h"

void IOController::init()
{
    Serial.println("Initializing IO controller");

    init_floors_input();
}

void IOController::init_floors_input()
{
    pinMode(FLOORS_REGISTERS_CLOCK, OUTPUT);
    pinMode(FLOORS_REGISTERS_LATCH, OUTPUT);

    pinMode(FLOORS_UP_PIN, INPUT);
    pinMode(FLOORS_DOWN_PIN, INPUT);
    pinMode(FLOORS_IR_PIN, INPUT);
}

void IOController::enable_floors_input()
{
    digitalWrite(FLOORS_REGISTERS_LATCH, 1);
}

void IOController::disable_floors_input()
{
    digitalWrite(FLOORS_REGISTERS_LATCH, 0);
}

void IOController::read_floors_input()
{
    enable_floors_input();
    delayMicroseconds(20);
    disable_floors_input();

    for (uint8_t i = FLOORS_REGISTERS_SIZE - 1; i >= 0; i--)
    {
        digitalWrite(FLOORS_REGISTERS_CLOCK, 0);
        delayMicroseconds(0.5);

        floorsUpButton[i] = digitalRead(FLOORS_UP_PIN);
        floorsDownButton[i] = digitalRead(FLOORS_DOWN_PIN);
        floorsIRSensor[i] = digitalRead(FLOORS_IR_PIN);

        digitalWrite(FLOORS_REGISTERS_CLOCK, 1);
    }
}

void IOController::read_elevator_numpad()
{
    int ADC_value = analogRead(ELEVATOR_NUMPAD_PIN);
    if (ADC_value >= (Key_1 - Hysteresis_1) && ADC_value <= (Key_1 + Hysteresis_1))
    {
        elevatorNumpad[0] = 1;
    }
    else if (ADC_value >= (Key_2 - Hysteresis_2) && ADC_value <= (Key_2 + Hysteresis_2))
    {
        elevatorNumpad[1] = 1;
    }
    else if (ADC_value >= (Key_3 - Hysteresis_3) && ADC_value <= (Key_3 + Hysteresis_3))
    {
        elevatorNumpad[2] = 1;
    }
    else if (ADC_value >= (Key_4 - Hysteresis_4) && ADC_value <= (Key_4 + Hysteresis_4))
    {
        elevatorNumpad[3] = 1;
    }
    else if (ADC_value >= (Key_5 - Hysteresis_5) && ADC_value <= (Key_5 + Hysteresis_5))
    {
        elevatorNumpad[4] = 1;
    }
    else if (ADC_value >= (Key_6 - Hysteresis_6) && ADC_value <= (Key_6 + Hysteresis_6))
    {
        elevatorNumpad[5] = 1;
    }
    else if (ADC_value >= (Key_7 - Hysteresis_7) && ADC_value <= (Key_7 + Hysteresis_7))
    {
        elevatorNumpad[6] = 1;
    }
    else if (ADC_value >= (Key_8 - Hysteresis_8) && ADC_value <= (Key_8 + Hysteresis_8))
    {
        elevatorNumpad[7] = 1;
    }
    /*else if (ADC_value >= (Key_9 - Hysteresis_9) && ADC_value <= (Key_9 + Hysteresis_9))
    {
        //Serial.println(9);
    }*/
}

void IOController::display_input()
{
    Serial.print("Up: ");
    for (auto bit : floorsUpButton)
    {
        Serial.print(bit);
    }
    Serial.print("\nDown: ");
    for (auto bit : floorsDownButton)
    {
        Serial.print(bit);
    }
    Serial.print("\nIR: ");
    for (auto bit : floorsIRSensor)
    {
        Serial.print(bit);
    }
    Serial.println("\nNumpad: ");
    for (auto bit : elevatorNumpad)
    {
        Serial.print(bit);
    }
    Serial.println(" ");
}
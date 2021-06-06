#include "IOController.h"

#define Key_1 93  // Corresponding ADC value for key 1
#define Key_2 171 // Corresponding ADC value for key 2
#define Key_3 236 // Corresponding ADC value for key 3
#define Key_4 293 // Corresponding ADC value for key 4
#define Key_5 341 // Corresponding ADC value for key 5
#define Key_6 384 // Corresponding ADC value for key 6
#define Key_7 422 // Corresponding ADC value for key 7
#define Key_8 455 // Corresponding ADC value for key 8
#define Key_9 485 // Corresponding ADC value for key 9

#define Hysteresis_1 30 // key 1 hysteresis
#define Hysteresis_2 25 // key 2 hysteresis
#define Hysteresis_3 20 // key 3 hysteresis
#define Hysteresis_4 18 // key 4 hysteresis
#define Hysteresis_5 15 // key 5 hysteresis
#define Hysteresis_6 14 // key 6 hysteresis
#define Hysteresis_7 12 // key 7 hysteresis
#define Hysteresis_8 10 // key 8 hysteresis
#define Hysteresis_9 8  // key 9 hysteresis

#define SET_BIT(REG, BIT) (REG |= (1 << BIT))
#define CLEAR_BIT(REG, BIT) (REG &= (~(1 << BIT)))

#define BIT_IS_SET(REG, BIT) (REG & (1 << BIT))
#define BIT_IS_CLEAR(REG, BIT) (!(REG & (1 << BIT)))

void IOController::init()
{
    initFloorsInput();
    init7Segment();
}

void IOController::initFloorsInput()
{
    pinMode(FLOORS_REGISTERS_CLOCK, OUTPUT);
    pinMode(FLOORS_REGISTERS_LATCH, OUTPUT);

    pinMode(FLOORS_UP_PIN, INPUT);
    pinMode(FLOORS_DOWN_PIN, INPUT);
    pinMode(FLOORS_IR_PIN, INPUT);
}

void IOController::init7Segment()
{
    pinMode(SEV_SEGMENT_REGISTER_LATCH, OUTPUT);
    pinMode(SEV_SEGMENT_REGISTER_CLOCK, OUTPUT);
    pinMode(SEV_SEGMENT_REGISTER_DATA, OUTPUT);
}

void IOController::enableFloorsInput()
{
    digitalWrite(FLOORS_REGISTERS_LATCH, 1);
}

void IOController::disableFloorsInput()
{
    digitalWrite(FLOORS_REGISTERS_LATCH, 0);
}

void IOController::readFloorsInput()
{
    enableFloorsInput();
    delayMicroseconds(20);
    disableFloorsInput();

    for (int i = FLOORS_REGISTERS_SIZE - 1; i >= 0; i--)
    {
        digitalWrite(FLOORS_REGISTERS_CLOCK, 0);
        delayMicroseconds(20);

        floorsUpButton[i] = digitalRead(FLOORS_UP_PIN);
        floorsDownButton[i] = digitalRead(FLOORS_DOWN_PIN);
        floorsIRSensor[i] = digitalRead(FLOORS_IR_PIN);

        digitalWrite(FLOORS_REGISTERS_CLOCK, 1);
    }
}

void IOController::readElevatorNumpad()
{
    // Reset elevator numpad input
    for (int i = 0; i < NUM_FLOORS; i++)
    {
        elevatorNumpad[i] = 0;
    }
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

Input<NUM_FLOORS> IOController::readInput()
{
    readElevatorNumpad();
    readFloorsInput();
    return {floorsUpButton, floorsDownButton, floorsIRSensor, elevatorNumpad};
}

void IOController::displayInput()
{
    Serial.print("Up: ");
    for (auto bit : floorsUpButton)
    {
        Serial.print(bit);
    }
    Serial.println("");
    Serial.print("Down: ");
    for (auto bit : floorsDownButton)
    {
        Serial.print(bit);
    }
    Serial.println("");
    Serial.print("IR: ");
    for (auto bit : floorsIRSensor)
    {
        Serial.print(bit);
    }
    Serial.println("");
    Serial.print("Numpad: ");
    for (auto bit : elevatorNumpad)
    {
        Serial.print(bit);
    }
    Serial.println("");
}

void IOController::enable7SegmentOutput()
{
    digitalWrite(SEV_SEGMENT_REGISTER_LATCH, 0);
}
void IOController::disable7SegmentOutput()
{
    digitalWrite(SEV_SEGMENT_REGISTER_LATCH, 1);
}

byte IOController::numberToBCD(byte number)
{
    byte seven_segments_value = 0;
    int j = SEVEN_SEGMENT_REGISTERS_SIZE;
    if (number > 0 && number <= 99)
    {
        for (int i = 3; i >= 0; i--)
        {
            j--;
            if (BIT_IS_SET((number - (number % 10)) / 10, i))
            {
                SET_BIT(seven_segments_value, j);
            }
        }
        for (int i = 3; i >= 0; i--)
        {
            j--;
            if (BIT_IS_SET((number % 10), i))
            {
                SET_BIT(seven_segments_value, j);
            }
        }
    }
    return seven_segments_value;
}

void IOController::output7Segment(byte output_data)
{
    byte seven_segments_value = numberToBCD(output_data);
   
    enable7SegmentOutput();

    for (int i = 0; i < SEVEN_SEGMENT_REGISTERS_SIZE; i++)
    {
        digitalWrite(SEV_SEGMENT_REGISTER_CLOCK, 0);
        digitalWrite(SEV_SEGMENT_REGISTER_DATA, seven_segments_value & (1 << i));
        digitalWrite(SEV_SEGMENT_REGISTER_CLOCK, 1);
        // Zero the data pin after shift to prevent bleed through
        digitalWrite(SEV_SEGMENT_REGISTER_DATA, 0);
    }
    // Stop shifting
    digitalWrite(SEV_SEGMENT_REGISTER_CLOCK, 0);

    disable7SegmentOutput();
}
#include "IOController.h"

#define SET_BIT(REG, BIT) (REG |= (1 << BIT))
#define CLEAR_BIT(REG, BIT) (REG &= (~(1 << BIT)))

#define BIT_IS_SET(REG, BIT) (REG & (1 << BIT))
#define BIT_IS_CLEAR(REG, BIT) (!(REG & (1 << BIT)))

#define INVALID_KEY -1

int keys[KEYPAD_ROW_NUM][KEYPAD_COLUMN_NUM] = {
    {1, 2, 3},
    {4, 5, 6},
    {7, INVALID_KEY, INVALID_KEY},
    {INVALID_KEY, 0, INVALID_KEY}
};

void IOController::init()
{
    pinMode(REGISTERS_CLOCK, OUTPUT);
    initKeypad();
    initFloorsInput();
    init7Segment();
}

void IOController::initKeypad() {
    pinMode(KEYPAD_IN_PIN, INPUT);
    pinMode(KEYPAD_OUT_PIN, OUTPUT);
    pinMode(KEYPAD_OUT_LATCH, OUTPUT);
}

void IOController::initFloorsInput()
{
    pinMode(FLOORS_INPUT_LATCH, OUTPUT);

    pinMode(FLOORS_UP_PIN, INPUT);
    pinMode(FLOORS_DOWN_PIN, INPUT);
    pinMode(FLOORS_IR_PIN, INPUT);
}

void IOController::init7Segment()
{
    pinMode(SEV_SEGMENT_REGISTER_LATCH, OUTPUT);
    pinMode(SEV_SEGMENT_REGISTER_DATA, OUTPUT);
}

void IOController::enableFloorsInput()
{
    digitalWrite(FLOORS_INPUT_LATCH, 1);
}

void IOController::disableFloorsInput()
{
    digitalWrite(FLOORS_INPUT_LATCH, 0);
}

void IOController::enableKeypadIn()
{
    digitalWrite(KEYPAD_IN_LATCH, 1);
}

void IOController::disableKeypadIn()
{
    digitalWrite(KEYPAD_IN_LATCH, 0);
}

void IOController::enable7SegmentOutput()
{
    digitalWrite(SEV_SEGMENT_REGISTER_LATCH, 0);
}
void IOController::disable7SegmentOutput()
{
    digitalWrite(SEV_SEGMENT_REGISTER_LATCH, 1);
}

void IOController::enableKeypadOut()
{
    digitalWrite(KEYPAD_OUT_LATCH, 0);
}

void IOController::disableKeypadOut()
{
    digitalWrite(KEYPAD_OUT_LATCH, 1);
}


void IOController::readFloorsInput()
{
    disableFloorsInput();
    for (int i = FLOORS_REGISTERS_SIZE - 1; i >= 0; i--)
    {
        digitalWrite(REGISTERS_CLOCK, 0);
        delayMicroseconds(20);

        floorsUpButton[i] = digitalRead(FLOORS_UP_PIN);
        floorsDownButton[i] = digitalRead(FLOORS_DOWN_PIN);
        floorsIRSensor[i] = digitalRead(FLOORS_IR_PIN);

        digitalWrite(REGISTERS_CLOCK, 1);
    }
    enableFloorsInput();
}

Bits<KEYPAD_COLS_REGISTER_SIZE> IOController::readKeypadColumns() {
    disableKeypadIn();
    Bits<KEYPAD_COLS_REGISTER_SIZE> keypadCols;
    for (int i = KEYPAD_COLS_REGISTER_SIZE - 1; i >= 0; i--)
    {
        digitalWrite(REGISTERS_CLOCK, 0);
        delayMicroseconds(20);
        keypadCols[i] = digitalRead(KEYPAD_IN_PIN);
        digitalWrite(REGISTERS_CLOCK, 1);
    }
    enableKeypadIn();
    return keypadCols;
}

void IOController::writeKeypadRow(uint8_t rowIndex)
{
    enableKeypadOut();
    for (int i = KEYPAD_ROWS_REGISTER_SIZE - 1; i >= 0; i--)
    {
        digitalWrite(REGISTERS_CLOCK, 0);
        digitalWrite(KEYPAD_OUT_PIN, rowIndex == i);
        digitalWrite(REGISTERS_CLOCK, 1);
        // Zero the data pin after shift to prevent bleed through
        digitalWrite(KEYPAD_OUT_PIN, 0);
    }
    // Stop shifting
    digitalWrite(REGISTERS_CLOCK, 0);
    disableKeypadOut();
}

void IOController::readKeypad()
{
    for(int row = 0; row < KEYPAD_ROW_NUM; row++) {
        writeKeypadRow(row);
        auto keypadColumns = readKeypadColumns();
        for(int col = 0; col < KEYPAD_COLUMN_NUM; col++) {
            auto key = keys[row][col];
            if (key == INVALID_KEY) continue;
            elevatorNumpad[keys[row][col]] = keypadColumns[col];
        }
    }
    writeKeypadRow(UINT8_MAX);
}

Input<NUM_FLOORS> IOController::readInput()
{
    readKeypad();
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
        digitalWrite(REGISTERS_CLOCK, 0);
        digitalWrite(SEV_SEGMENT_REGISTER_DATA, seven_segments_value & (1 << i));
        digitalWrite(REGISTERS_CLOCK, 1);
        // Zero the data pin after shift to prevent bleed through
        digitalWrite(SEV_SEGMENT_REGISTER_DATA, 0);
    }
    // Stop shifting
    digitalWrite(REGISTERS_CLOCK, 0);

    disable7SegmentOutput();
}
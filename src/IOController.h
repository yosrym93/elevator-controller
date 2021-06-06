#pragma once
#include <BitBool.h>
#include "Arduino.h"
#include "nextState.h"
#include "constants.h"

#define Bits BitBool

class IOController
{
    Bits<NUM_FLOORS> floorsUpButton;
    Bits<NUM_FLOORS> floorsDownButton;
    Bits<NUM_FLOORS> floorsIRSensor;
    Bits<NUM_FLOORS> elevatorNumpad;

    void initKeypad();
    void initFloorsInput();
    void init7Segment();

    void enableFloorsInput();
    void disableFloorsInput();

    void enableKeypadIn();
    void disableKeypadIn();

    void enableKeypadOut();
    void disableKeypadOut();
    
    void enable7SegmentOutput();
    void disable7SegmentOutput();

    Bits<KEYPAD_COLS_REGISTER_SIZE> readKeypadColumns();
    void writeKeypadRow(uint8_t rowIndex);

    void readFloorsInput();
    void readKeypad();
    
    byte numberToBCD(byte number);
public:
    void init();
    Input<NUM_FLOORS> readInput();
    void output7Segment(uint8_t value);
    void displayInput();
   
};
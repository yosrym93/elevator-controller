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

    void initFloorsInput();
    void init7Segment();

    void enableFloorsInput();
    void disableFloorsInput();
    
    void enable7SegmentOutput();
    void disable7SegmentOutput();

    void readFloorsInput();
    void readElevatorNumpad();
    
    byte numberToBCD(byte number);
public:
    void init();
    Input<NUM_FLOORS> readInput();
    void output7Segment(uint8_t value);
    void displayInput();
   
};
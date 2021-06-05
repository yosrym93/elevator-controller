#pragma once
#include <BitBool.h>

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
    void enableFloorsInput();
    void disableFloorsInput();
    void readFloorsInput();
    void readElevatorNumpad();
public:
    void init();
    Input<NUM_FLOORS> readInput();
    void displayInput();
};
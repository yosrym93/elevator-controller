#pragma once
#include <BitBool.h>

#define Bits BitBool

#define NumFloors   8

#define FLOORS_REGISTERS_CLOCK A0
#define FLOORS_REGISTERS_LATCH A1
#define FLOORS_UP_PIN A2
#define FLOORS_DOWN_PIN A3
#define FLOORS_IR_PIN A4

class IOController
{
    void init_floors_input();
    void enable_floors_input();
    void disable_floors_input();

public:
    Bits<NumFloors> floorsUpButton;
    Bits<NumFloors> floorsDownButton;
    Bits<NumFloors> floorsIRSensor;

    void init();
    void read_floors_input();
    void display_floors_input();
};
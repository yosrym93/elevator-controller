#pragma once
#include <BitBool.h>

#define Bits BitBool

#define NumFloors   8
#define FLOORS_REGISTERS_SIZE 8

#define FLOORS_REGISTERS_CLOCK A0
#define FLOORS_REGISTERS_LATCH A1
#define FLOORS_UP_PIN A2
#define FLOORS_DOWN_PIN A3
#define FLOORS_IR_PIN A4

#define ELEVATOR_NUMPAD_PIN A5

#define Key_1 93        // Corresponding ADC value for key 1
#define Key_2 171       // Corresponding ADC value for key 2
#define Key_3 236       // Corresponding ADC value for key 3
#define Key_4 293       // Corresponding ADC value for key 4
#define Key_5 341       // Corresponding ADC value for key 5
#define Key_6 384       // Corresponding ADC value for key 6
#define Key_7 422       // Corresponding ADC value for key 7
#define Key_8 455       // Corresponding ADC value for key 8
#define Key_9 485       // Corresponding ADC value for key 9

#define Hysteresis_1 30       // key 1 hysteresis
#define Hysteresis_2 25       // key 2 hysteresis
#define Hysteresis_3 20       // key 3 hysteresis
#define Hysteresis_4 18       // key 4 hysteresis
#define Hysteresis_5 15       // key 5 hysteresis
#define Hysteresis_6 14       // key 6 hysteresis
#define Hysteresis_7 12       // key 7 hysteresis
#define Hysteresis_8 10       // key 8 hysteresis
#define Hysteresis_9 8        // key 9 hysteresis

class IOController
{
    void initFloorsInput();
    void enableFloorsInput();
    void disableFloorsInput();

public:
    Bits<NumFloors> floorsUpButton;
    Bits<NumFloors> floorsDownButton;
    Bits<NumFloors> floorsIRSensor;
    // TODO: Rememeber to reset this vector
    Bits<NumFloors> elevatorNumpad;

    void init();
    void readFloorsInput();
    void readElevatorNumpad();
    void displayInput();
};
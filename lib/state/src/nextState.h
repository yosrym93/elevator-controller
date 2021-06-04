#pragma once

#ifdef UNIT_TEST
#include <array>

template<uint8_t N>
using Bits = std::array<bool, N>;
#else
#include <BitBool.h>

#define Bits BitBool
#endif

// assuming buttons are not sticky
// Bi=1 as long as someone is pressing, otherwise Bi=0
template<uint8_t NumFloors>
struct Input {
    // every floor has one floorUpButton + one floorDownButton + one door sensor
    Bits<NumFloors> floorUpButtons, floorDownButtons, doorsSensors;

    // numpad has buttons = #levels
    Bits<NumFloors> numpad;
};

enum class Direction: uint8_t {
    STOP = 0, UP, DOWN
};

template<uint8_t NumFloors>
struct State {
    // remember the state of the buttons
    Bits<NumFloors> floorUpButtons, floorDownButtons, numpad;

    // don't move if currentFloor == nextFloor
    uint8_t currentFloor, nextFloor;

    Direction direction;
};

// initial state is all zeroed out
// use it to declare state: State<N> s = ZERO_STRUCT;
#define ZERO_STRUCT {0}

template<uint8_t NumFloors>
inline State<NumFloors> nextState(Input<NumFloors> const* in, State<NumFloors> const* curr) {
    // TODO
    return ZERO_STRUCT;
}

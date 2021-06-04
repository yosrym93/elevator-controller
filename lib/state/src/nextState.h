#pragma once

#ifdef UNIT_TEST
#include <array>

template<size_t N>
using Bits = std::array<bool, N>;
#else
#include <BitBool.h>

#define Bits BitBool
#endif

// assuming buttons are not sticky
// Bi=1 as long as someone is pressing, otherwise Bi=0
template<size_t NumFloors>
struct Input {
    // every floor has one floorUpButton + one floorDownButton + one door sensor
    Bits<NumFloors> floorUpButtons, floorDownButtons, doorsSensors;

    // numpad has buttons = #levels
    Bits<NumFloors> numpad;
};

template<size_t NumFloors>
struct State {
    // remember the state of the buttons
    Bits<NumFloors> floorUpButtons, floorDownButtons, numpad;

    // don't move if currentFloor == nextFloor
    int currentFloor, nextFloor;
};

// initial state is all zeroed out
// use it to declare state: State<N> s = ZERO_STRUCT;
#define ZERO_STRUCT {0}

template<size_t NumFloors>
inline State<NumFloors> nextState(Input<NumFloors> const* in, State<NumFloors> const* curr) {
    // TODO
    return ZERO_STRUCT;
}

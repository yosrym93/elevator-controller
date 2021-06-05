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

    // numpad has buttons = #floors
    Bits<NumFloors> numpad;
};

enum class Direction: uint8_t {
    STOP = 0, UP, DOWN
};

// initial state is all zeroed out
// use it to declare state: State<N> s = ZERO_STRUCT;
#define ZERO_STRUCT {0}

template<uint8_t NumFloors>
struct State {
    // remember the state of the buttons
    Bits<NumFloors> floorUpButtons, floorDownButtons, numpad;

    // Function getNextState doesn't change currentFloor.
    // Set currentFloor with new value when the elevator reaches that floor
    // before calling getNextState.
    // Don't move elevator if currentFloor == nextFloor.
    uint8_t currentFloor;
    uint8_t nextFloor;

    Direction getDirection() const;
    State<NumFloors> getNextState(Input<NumFloors> const* input) const;
};

template<uint8_t NumFloors>
Direction State<NumFloors>::getDirection() const {
    if (nextFloor == currentFloor) {
        return Direction::STOP;
    } else if (nextFloor > currentFloor) {
        return Direction::UP;
    }

    return Direction::DOWN;
}

template<uint8_t N>
Bits<N> OrBits(Bits<N> a, Bits<N> b) {
    Bits<N> c;
    for (uint8_t i = 0; i < N; i++) {
        c[i] = a[i] | b[i];
    }
    return c;
}

template<uint8_t NumFloors>
State<NumFloors> State<NumFloors>::getNextState(Input<NumFloors> const* input) const {
    // TODO
    return ZERO_STRUCT;
}

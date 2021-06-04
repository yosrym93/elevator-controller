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
template<size_t NumLevels>
struct Input {
    // every level has one floorUpButton + one floorDownButton + one door sensor
    Bits<NumLevels> floorUpButtons, floorDownButtons, doorsSensors;

    // numpad has buttons = #levels
    Bits<NumLevels> numpad;
};

template<size_t NumLevels>
struct State {
    // remember the state of the buttons
    Bits<NumLevels> floorUpButtons, floorDownButtons, numpad;

    // don't move if currentLevel == nextLevel
    int currentLevel, nextLevel;
};

// initial state is all zeroed out
// use it to declare state: State<N> s = ZERO_STRUCT;
#define ZERO_STRUCT {0}

template<size_t NumLevels>
inline void nextState(Input<NumLevels> const* in, State<NumLevels> const* curr, State<NumLevels>* next) {
    // TODO
}

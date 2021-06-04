#pragma once

#include "Bits.h"

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

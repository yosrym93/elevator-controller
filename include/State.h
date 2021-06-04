#pragma once

#include "Bits.h"

template<size_t NumLevels>
struct State {
    // remember the state of the buttons
    Bits<NumLevels> floorUpButtons, floorDownButtons, numpad;

    // don't move if currentLevel == nextLevel
    int currentLevel, nextLevel;
};

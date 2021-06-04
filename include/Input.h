#pragma once

#include "Bits.h"

// assuming buttons are not sticky
// Bi=1 as long as someone is pressing, otherwise Bi=0
template<size_t NumLevels>
struct Input {
    // every level has one floorUpButton + one floorDownButton + one door sensor
    Bits<NumLevels> floorUpButtons, floorDownButtons, doorsSensors;

    // numpad has buttons = #levels
    Bits<NumLevels> numpad;
};

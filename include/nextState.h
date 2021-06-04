#pragma once

#include "Input.h"
#include "State.h"

template<size_t NumLevels>
void nextState(Input<NumLevels> const* in, State const* curr, State* next);

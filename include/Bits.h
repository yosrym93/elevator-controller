#pragma once

#ifdef UNIT_TEST
#include <array>

template<size_t N>
using Bits = std::array<bool, N>;
#else
#include <BitBool.h>

#define Bits BitBool
#endif
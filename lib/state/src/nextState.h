#pragma once

#include <limits.h>
#include <stdlib.h>

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
struct NextStopPlanningState {
    // remember the state of the buttons
    Bits<NumFloors> reqUp, reqDown, numpad;

    // Function getNextState doesn't change currentFloor.
    // Set currentFloor with new value when the elevator reaches that floor
    // before calling getNextState.
    // Don't move elevator if currentFloor == nextFloor.

    bool isMoving;
    uint8_t currentFloor;
    uint8_t nextFloor;

    NextStopPlanningState<NumFloors> getNextState(Input<NumFloors> const* input) const;
};

inline Direction getDirection(uint8_t currentFloor, uint8_t nextFloor) {
    if (nextFloor == currentFloor) {
        return Direction::STOP;
    }
    
    if (nextFloor > currentFloor) {
        return Direction::UP;
    }

    return Direction::DOWN;
}

template<uint8_t N>
Bits<N> OrBits(Bits<N> const& a, Bits<N> const& b) {
    Bits<N> c;
    for (uint8_t i = 0; i < N; i++) {
        c[i] = a[i] | b[i];
    }
    return c;
}

template<uint8_t N>
Bits<N> AndBits(Bits<N> const& a, Bits<N> const& b) {
    Bits<N> c;
    for (uint8_t i = 0; i < N; i++) {
        c[i] = a[i] & b[i];
    }
    return c;
}

template<uint8_t N>
uint8_t getIndexOfNextHighBit(Bits<N> a, uint8_t i) {
    for (int j = i+1; j < N; j++) {
        if (a[j]) {
            return j;
        }
    }
    return i;
}

template<uint8_t N>
uint8_t getIndexOfPrevHighBit(Bits<N> a, uint8_t i) {
    for (int j = i-1; j >= 0; j--) {
        if (a[j]) {
            return j;
        }
    }
    return i;
}

inline bool canMoveFromTo(uint8_t curFloor, uint8_t comFloor) {
    return curFloor != comFloor;
}

// return one of [a,b,c,d] which has min dist to x
inline uint8_t minDistance(uint8_t x, uint8_t a, uint8_t b, uint8_t c, uint8_t d) {
    uint8_t choice = x;
    int m = INT_MAX;
    int arr[] = {a, b, c, d};

    for (int i = 0; i < 4; i++) {
        auto m2 = abs(int(x)-int(arr[i]));
        if (m2 != 0 && m2 < m) {
            m = m2;
            choice = arr[i];
        }
    }

    return choice;
}

// return first non zero distance to x in [a,b,c,d]
inline uint8_t firstDistance(uint8_t x, uint8_t a, uint8_t b, uint8_t c, uint8_t d) {
    int arr[] = {a, b, c, d};
    for (int i = 0; i < 4; i++) {
        if (arr[i] != x) {
            return arr[i];
        }
    }
    return x;
}

inline uint8_t safeAddToByte(uint8_t i, int toAdd) {
    int j = int(i) + toAdd;

    if (j > 0xFF) {
        return 0xFF;
    }

    if (j < 0) {
        return 0;
    }

    return uint8_t(j);
}

template<uint8_t NumFloors>
NextStopPlanningState<NumFloors> NextStopPlanningState<NumFloors>::getNextState(Input<NumFloors> const* input) const {
    auto numpad = OrBits<NumFloors>(this->numpad, input->numpad);

    // reqestedButton = (buttons | input->buttons & input->IR) | numpad
    // this way if no one in floor, the button state will be zero
    auto reqUp = OrBits<NumFloors>(AndBits<NumFloors>(OrBits<NumFloors>(this->reqUp, input->floorUpButtons), input->doorsSensors), numpad);
    auto reqDown = OrBits<NumFloors>(AndBits<NumFloors>(OrBits<NumFloors>(this->reqDown, input->floorDownButtons), input->doorsSensors), numpad);

    // clear current floor from numpad and requested floors
    if (!isMoving) {
        numpad[currentFloor] = 0;
        reqUp[currentFloor] = 0;
        reqDown[currentFloor] = 0;
    }

    // next and prev requested floors
    const auto nextUp   = getIndexOfNextHighBit<NumFloors>(reqUp, currentFloor);
    const auto prevUp   = getIndexOfPrevHighBit<NumFloors>(reqUp, currentFloor);
    const auto nextDown = getIndexOfNextHighBit<NumFloors>(reqDown, currentFloor);
    const auto prevDown = getIndexOfPrevHighBit<NumFloors>(reqDown, currentFloor);

    auto nextFloor = this->nextFloor;
    uint8_t movingToFloor;

    switch (const auto dir = getDirection(currentFloor, nextFloor)) {
    case Direction::UP:
        nextFloor = firstDistance(currentFloor, nextUp, nextDown, prevUp, prevDown);
        movingToFloor = safeAddToByte(currentFloor, +1);
        break;
    case Direction::DOWN:
        nextFloor = firstDistance(currentFloor, prevDown, prevUp, nextDown, nextUp);
        movingToFloor = safeAddToByte(currentFloor, -1);
        break;
    case Direction::STOP:
        nextFloor = minDistance(currentFloor, nextUp, nextDown, prevDown, prevUp);
        movingToFloor = currentFloor;
        break;
    }

    // should stop
    if (isMoving && getDirection(currentFloor, nextFloor) == Direction::STOP) {
        // stop at floor im moving to
        nextFloor = uint8_t(movingToFloor);
    }

    return NextStopPlanningState<NumFloors> {
        reqUp: reqUp,
        reqDown: reqDown,
        numpad: numpad,
        isMoving: isMoving,
        currentFloor: currentFloor,
        nextFloor: nextFloor,
    };
}

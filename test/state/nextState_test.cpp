#include <unity.h>
#include <nextState.h>

constexpr size_t NUM_FLOORS = 5;
NextStopPlanningState<NUM_FLOORS> st;
Input<NUM_FLOORS> in;

inline void updateAndCheckState(void) {
    uint8_t _floor = st.currentFloor;
    st = st.getNextState(&in);
    in = ZERO_STRUCT;
    // shouldn't change the currentFloor
    TEST_ASSERT_EQUAL(_floor, st.currentFloor);
}

void test_or_bits(void) {
    const auto size = 9;
    auto a         = Bits<size>{1,0,0, 1,0,0, 1,0,0};
    auto b         = Bits<size>{0,1,0, 1,0,0, 0,0,0};
    auto expectedC = Bits<size>{1,1,0, 1,0,0, 1,0,0};

    auto c = OrBits<size>(a, b);
    for (int i = 0; i < size; i++) {
        TEST_ASSERT_EQUAL(expectedC[i], c[i]);
    }
}

void test_and_bits(void) {
    const auto size = 9;
    auto a         = Bits<size>{1,0,0, 1,0,0, 1,0,0};
    auto b         = Bits<size>{0,1,0, 1,0,0, 0,0,0};
    auto expectedC = Bits<size>{0,0,0, 1,0,0, 0,0,0};

    auto c = AndBits<size>(a, b);
    for (int i = 0; i < size; i++) {
        TEST_ASSERT_EQUAL(expectedC[i], c[i]);
    }
}

void test_getIndexOfNextHighBit(void) {
    {
        const auto size = 9;
        auto a         = Bits<size>{1,0,0, 1,0,0, 1,0,0};
        auto i = 2;
        auto expectedI = 3;

        auto actualI = getIndexOfNextHighBit<size>(a, i);
        TEST_ASSERT_EQUAL(expectedI, actualI);
    }

    {
        const auto size = 9;
        auto a         = Bits<size>{1,0,0, 1,0,0, 1,0,0};
        auto i = 3;
        auto expectedI = 6;

        auto actualI = getIndexOfNextHighBit<size>(a, i);
        TEST_ASSERT_EQUAL(expectedI, actualI);
    }

    {
        const auto size = 9;
        auto a         = Bits<size>{1,0,0, 1,0,0, 1,0,0};
        auto i = 7;
        auto expectedI = 7;

        auto actualI = getIndexOfNextHighBit<size>(a, i);
        TEST_ASSERT_EQUAL(expectedI, actualI);
    }
}

void test_getIndexOfPrevHighBit(void) {
    {
        const auto size = 9;
        auto a         = Bits<size>{1,0,0, 1,0,0, 1,0,0};
        auto i = 2;
        auto expectedI = 0;

        auto actualI = getIndexOfPrevHighBit<size>(a, i);
        TEST_ASSERT_EQUAL(expectedI, actualI);
    }

    {
        const auto size = 9;
        auto a         = Bits<size>{1,0,0, 1,0,0, 1,0,0};
        auto i = 4;
        auto expectedI = 3;

        auto actualI = getIndexOfPrevHighBit<size>(a, i);
        TEST_ASSERT_EQUAL(expectedI, actualI);
    }

    {
        const auto size = 9;
        auto a         = Bits<size>{0,0,0, 1,0,0, 1,0,0};
        auto i = 1;
        auto expectedI = 1;

        auto actualI = getIndexOfPrevHighBit<size>(a, i);
        TEST_ASSERT_EQUAL(expectedI, actualI);
    }
}

void setUp(void) {
    st = ZERO_STRUCT;
    in = ZERO_STRUCT;
}

void test_zeroed(void) {
    TEST_ASSERT_EQUAL(0, st.currentFloor);
    TEST_ASSERT_EQUAL(0, st.nextFloor);
    for (auto i : st.reqDown) {
        TEST_ASSERT_EQUAL(0, i);
    }
    for (auto i : st.reqUp) {
        TEST_ASSERT_EQUAL(0, i);
    }
    for (auto i : st.numpad) {
        TEST_ASSERT_EQUAL(0, i);
    }
}

void test_scenario_0(void) {
    // 1 person on floor 0
    in.doorsSensors[0] = 1;
    // push up
    in.floorUpButtons[0] = 1;
    updateAndCheckState();
    TEST_ASSERT_EQUAL(0, st.nextFloor);
    TEST_ASSERT_EQUAL(0, st.reqUp[0]);

    // person goes on elv
    in.floorUpButtons[0] = in.doorsSensors[0] = 0;
    // pushes 1
    in.numpad[1] = 1;
    updateAndCheckState();
    TEST_ASSERT_EQUAL(1, st.nextFloor);
    TEST_ASSERT_EQUAL(0, st.reqUp[0]);
    TEST_ASSERT_EQUAL(1, st.numpad[1]);

    // elv reaches floor 1
    st.currentFloor = 1;
    updateAndCheckState();
    TEST_ASSERT_EQUAL(1, st.nextFloor);
    TEST_ASSERT_EQUAL(0, st.reqUp[0]);
    TEST_ASSERT_EQUAL(0, st.numpad[1]);
}

void test_scenario_1(void) {
    // 2 persons on floor 0
    in.doorsSensors[0] = 1;
    // push up
    in.floorUpButtons[0] = 1;
    updateAndCheckState();
    TEST_ASSERT_EQUAL(0, st.nextFloor);
    TEST_ASSERT_EQUAL(0, st.reqUp[0]);

    // 1 person goes on elv
    in.floorUpButtons[0] = 0;
    // the other was still standing
    in.doorsSensors[0] = 1;
    // pushes 2
    in.numpad[2] = 1;
    updateAndCheckState();
    TEST_ASSERT_EQUAL(2, st.nextFloor);
    TEST_ASSERT_EQUAL(0, st.reqUp[0]);
    TEST_ASSERT_EQUAL(1, st.numpad[2]);

    // elv reaches floor 1
    st.currentFloor = 1;
    updateAndCheckState();
    TEST_ASSERT_EQUAL(2, st.nextFloor);
    TEST_ASSERT_EQUAL(0, st.reqUp[0]);
    TEST_ASSERT_EQUAL(1, st.numpad[2]);

    // elv reaches floor 2
    st.currentFloor = 2;
    updateAndCheckState();
    TEST_ASSERT_EQUAL(2, st.nextFloor);
    TEST_ASSERT_EQUAL(0, st.reqUp[0]);
    TEST_ASSERT_EQUAL(0, st.numpad[2]);
}

void test_scenario_2(void) {
    // 1 person on floor 1
    in.doorsSensors[1] = 1;
    // push down
    in.floorDownButtons[1] = 1;
    updateAndCheckState();
    TEST_ASSERT_EQUAL(1, st.nextFloor);
    TEST_ASSERT_EQUAL(1, st.reqDown[1]);

    // elv reached floor 1
    st.currentFloor = 1;
    // 1 person stepped in
    in.doorsSensors[1] = 0;
    in.floorDownButtons[1] = 0;
    // pushes 3
    in.numpad[3] = 1;
    updateAndCheckState();
    TEST_ASSERT_EQUAL(3, st.nextFloor);
    TEST_ASSERT_EQUAL(0, st.reqDown[1]);
    TEST_ASSERT_EQUAL(1, st.numpad[3]);

    // elv reaches floor 3
    st.currentFloor = 3;
    updateAndCheckState();
    TEST_ASSERT_EQUAL(3, st.nextFloor);
    TEST_ASSERT_EQUAL(0, st.reqDown[1]);
    TEST_ASSERT_EQUAL(0, st.numpad[3]);
}

void test_scenario_3(void) {
    // elv on floor 4
    st.currentFloor = 4;
    // 1 person on floor 1
    in.doorsSensors[1] = 1;
    // push down
    in.floorDownButtons[1] = 1;
    updateAndCheckState();
    TEST_ASSERT_EQUAL(1, st.nextFloor);
    TEST_ASSERT_EQUAL(1, st.reqDown[1]);

    // elv on floor 1
    st.currentFloor = 1;
    // person goes on elv
    in.doorsSensors[1] = 0;
    in.floorDownButtons[1] = 0;
    // pushes 0
    in.numpad[0] = 1;
    updateAndCheckState();
    TEST_ASSERT_EQUAL(0, st.nextFloor);
    TEST_ASSERT_EQUAL(0, st.reqDown[1]);
    TEST_ASSERT_EQUAL(1, st.numpad[0]);

    // elv reaches floor 0
    st.currentFloor = 0;
    updateAndCheckState();
    TEST_ASSERT_EQUAL(0, st.nextFloor);
    TEST_ASSERT_EQUAL(0, st.reqDown[1]);
    TEST_ASSERT_EQUAL(0, st.numpad[0]);
}

void test_scenario_4(void) {
    // elv on floor 4
    st.currentFloor = 4;
    st.nextFloor = 4;
    // 1 person on floor 1
    in.doorsSensors[1] = 1;
    // push down
    in.floorDownButtons[1] = 1;
    updateAndCheckState();
    TEST_ASSERT_EQUAL(1, st.nextFloor);
    TEST_ASSERT_EQUAL(1, st.reqDown[1]);

    // elv on floor 3
    st.currentFloor = 3;
    // person on floor 2
    in.doorsSensors[2] = 1;
    // pushes up
    in.floorUpButtons[2] = 1;
    // 1 person on floor 1
    in.doorsSensors[1] = 1;
    updateAndCheckState();
    TEST_ASSERT_EQUAL(1, st.nextFloor);
    TEST_ASSERT_EQUAL(1, st.reqUp[2]);
    TEST_ASSERT_EQUAL(1, st.reqDown[1]);

    // elv on floor 1
    st.currentFloor = 1;
    // person goes on elv
    in.doorsSensors[1] = 0;
    in.floorDownButtons[1] = 0;
    // pushes 2
    in.numpad[2] = 1;
    updateAndCheckState();
    TEST_ASSERT_EQUAL(2, st.nextFloor);
    TEST_ASSERT_EQUAL(1, st.reqUp[2]);
    TEST_ASSERT_EQUAL(0, st.reqDown[1]);
    TEST_ASSERT_EQUAL(1, st.numpad[2]);

    // elv reaches floor 2
    st.currentFloor = 2;
    // one goes in another goes out
    in.floorUpButtons[2] = 0;
    // pushes 4
    in.numpad[4] = 1;
    updateAndCheckState();
    TEST_ASSERT_EQUAL(4, st.nextFloor);
    TEST_ASSERT_EQUAL(0, st.reqUp[2]);
    TEST_ASSERT_EQUAL(0, st.reqDown[1]);
    TEST_ASSERT_EQUAL(0, st.numpad[2]);
    TEST_ASSERT_EQUAL(1, st.numpad[4]);

    // elv reaches floor 4
    st.currentFloor = 4;
    // one goes out
    in.doorsSensors[4] = 1;
    updateAndCheckState();
    TEST_ASSERT_EQUAL(4, st.nextFloor);
    TEST_ASSERT_EQUAL(0, st.reqUp[2]);
    TEST_ASSERT_EQUAL(0, st.reqDown[1]);
    TEST_ASSERT_EQUAL(0, st.numpad[2]);
    TEST_ASSERT_EQUAL(0, st.numpad[4]);
}

void test_scenario_5(void) {
    // elv on floor 4
    st.currentFloor = 4;
    st.nextFloor = 4;
    // 1 person on floor 1
    in.doorsSensors[1] = 1;
    // push down
    in.floorDownButtons[1] = 1;
    updateAndCheckState();
    TEST_ASSERT_EQUAL(1, st.nextFloor);
    TEST_ASSERT_EQUAL(1, st.reqDown[1]);

    // elv on floor 3
    st.currentFloor = 3;
    // person on floor 2
    in.doorsSensors[2] = 1;
    // pushes down
    in.floorDownButtons[2] = 1;
    // 1 person on floor 1
    in.doorsSensors[1] = 1;
    updateAndCheckState();
    TEST_ASSERT_EQUAL(2, st.nextFloor);
    TEST_ASSERT_EQUAL(1, st.reqDown[2]);
    TEST_ASSERT_EQUAL(1, st.reqDown[1]);

    // elv on floor 2
    st.currentFloor = 2;
    // person goes on elv
    in.doorsSensors[2] = 0;
    in.floorDownButtons[2] = 0;
    // pushes 0
    in.numpad[0] = 1;
    // 1 person on floor 1
    in.doorsSensors[1] = 1;
    updateAndCheckState();
    TEST_ASSERT_EQUAL(1, st.nextFloor);
    TEST_ASSERT_EQUAL(0, st.reqDown[2]);
    TEST_ASSERT_EQUAL(1, st.reqDown[1]);
    TEST_ASSERT_EQUAL(1, st.numpad[0]);

    // elv reaches floor 1
    st.currentFloor = 1;
    // one goes in
    in.doorsSensors[1] = 0;
    in.floorDownButtons[1] = 0;
    // pushes 0
    in.numpad[0] = 1;
    updateAndCheckState();
    TEST_ASSERT_EQUAL(0, st.nextFloor);
    TEST_ASSERT_EQUAL(0, st.reqDown[2]);
    TEST_ASSERT_EQUAL(0, st.reqDown[1]);
    TEST_ASSERT_EQUAL(1, st.numpad[0]);

    // elv reaches floor 0
    st.currentFloor = 0;
    // two go out
    in.doorsSensors[0] = 1;
    updateAndCheckState();
    TEST_ASSERT_EQUAL(0, st.nextFloor);
    TEST_ASSERT_EQUAL(0, st.reqDown[2]);
    TEST_ASSERT_EQUAL(0, st.reqDown[1]);
    TEST_ASSERT_EQUAL(0, st.numpad[0]);
}

void test_scenario_6(void) {
    // elv on floor 0
    st.currentFloor = 0;
    // 1 person on floor 1
    in.doorsSensors[1] = 1;
    // push up
    in.floorUpButtons[1] = 1;
    updateAndCheckState();
    TEST_ASSERT_EQUAL(1, st.nextFloor);
    TEST_ASSERT_EQUAL(1, st.reqUp[1]);

    // elv on floor 1
    st.currentFloor = 1;
    // person on floor 1 goes in
    in.doorsSensors[1] = 0;
    in.floorUpButtons[1] = 0;
    // pushes 4
    in.numpad[4] = 1;
    // person on floor 2
    in.doorsSensors[2] = 1;
    // pushes up
    in.floorUpButtons[2] = 1;
    updateAndCheckState();
    TEST_ASSERT_EQUAL(2, st.nextFloor);
    TEST_ASSERT_EQUAL(1, st.reqUp[2]);
    TEST_ASSERT_EQUAL(0, st.reqUp[1]);

    // elv on floor 2
    st.currentFloor = 2;
    // person goes on elv
    in.doorsSensors[2] = 0;
    in.floorUpButtons[2] = 0;
    // pushes 4
    in.numpad[4] = 1;
    updateAndCheckState();
    TEST_ASSERT_EQUAL(4, st.nextFloor);
    TEST_ASSERT_EQUAL(0, st.reqUp[2]);
    TEST_ASSERT_EQUAL(0, st.reqUp[1]);
    TEST_ASSERT_EQUAL(1, st.numpad[4]);

    // elv on floor 4
    st.currentFloor = 4;
    // 2 persons go out
    in.doorsSensors[4] = 1;
    updateAndCheckState();
    TEST_ASSERT_EQUAL(4, st.nextFloor);
    TEST_ASSERT_EQUAL(0, st.reqUp[2]);
    TEST_ASSERT_EQUAL(0, st.reqUp[1]);
    TEST_ASSERT_EQUAL(0, st.numpad[4]);
}

void test_scenario_7(void) {
    // elv on floor 2
    st.currentFloor = 2;
    // going to floor 4
    st.nextFloor = 4;
    // with one person
    st.numpad[4] = 1;
    // person on floor 3
    in.doorsSensors[3] = 1;
    // pushed up
    in.floorUpButtons[3] = 1;
    updateAndCheckState();
    TEST_ASSERT_EQUAL(3, st.nextFloor);

    // elv on floor 3
    st.currentFloor = 3;
    // person got on
    in.doorsSensors[3] = 0;
    // pushed numpad 4
    in.numpad[4] = 1;
    updateAndCheckState();
    TEST_ASSERT_EQUAL(4, st.nextFloor);
    TEST_ASSERT_EQUAL(1, st.numpad[4]);

    // elv on floor 4
    st.currentFloor = 4;
    // 2 persons got out
    in.doorsSensors[4] = 1;
    updateAndCheckState();
    TEST_ASSERT_EQUAL(4, st.nextFloor);
    TEST_ASSERT_EQUAL(0, st.numpad[4]);
}

void test_scenario_8(void) {
    // stupid person on final floor 
    in.doorsSensors[NUM_FLOORS-1] = 1;
    // pushed up
    in.floorUpButtons[NUM_FLOORS-1] = 1;
    updateAndCheckState();
    TEST_ASSERT_EQUAL(NUM_FLOORS-1, st.nextFloor);
}

void test_scenario_9(void) {
    // elv on final floor
    st.currentFloor = NUM_FLOORS-1;
    st.nextFloor = NUM_FLOORS-1;
    // stupid person on first floor 
    in.doorsSensors[0] = 1;
    // pushed down
    in.floorUpButtons[0] = 1;
    updateAndCheckState();
    TEST_ASSERT_EQUAL(0, st.nextFloor);
}

void test_scenario_10(void) {
    // elv in 2
    st.nextFloor = 2;
    st.currentFloor = 2;
    // person on floor 4
    in.doorsSensors[4] = 1;
    // pushed down
    in.floorDownButtons[4] = 1;
    updateAndCheckState();
    TEST_ASSERT_EQUAL(4, st.nextFloor);

    // elv between 2,3
    st.currentFloor = 2;
    st.isMoving = 1;
    // person on floor 2
    in.doorsSensors[2] = 1;
    // pushed down
    in.floorDownButtons[2] = 1;
    // person on floor 4
    in.doorsSensors[4] = 1;
    updateAndCheckState();
    TEST_ASSERT_EQUAL(4, st.nextFloor);

    // elv in 4
    st.currentFloor = 4;
    st.isMoving = 0;
    // person goes in, pushes 0
    in.numpad[0] = 1;
    // person on floor 2
    in.doorsSensors[2] = 1;
    updateAndCheckState();
    TEST_ASSERT_EQUAL(2, st.nextFloor);
    TEST_ASSERT_EQUAL(1, st.numpad[0]);

    // elv in 2
    st.currentFloor = 2;
    // person goes in, pushes 0
    in.numpad[0] = 1;
    updateAndCheckState();
    TEST_ASSERT_EQUAL(0, st.nextFloor);
    TEST_ASSERT_EQUAL(1, st.numpad[0]);
}

void test_scenario_11(void) {
    // person on floor 1
    in.doorsSensors[1] = 1;
    // pushed up
    in.floorUpButtons[1] = 1;
    // person on floor 2
    in.doorsSensors[2] = 1;
    // pushed up
    in.floorUpButtons[2] = 1;
    updateAndCheckState();
    TEST_ASSERT_EQUAL(1, st.nextFloor);

    // elv going to floor 1
    st.currentFloor = 0;
    st.isMoving = 1;
    // person on floor 1
    in.doorsSensors[1] = 1;
    // person on floor 2
    in.doorsSensors[2] = 1;
    updateAndCheckState();
    TEST_ASSERT_EQUAL(1, st.nextFloor);

    // elv in floor 1
    st.currentFloor = 1;
    st.isMoving = 0;
    // person goes on, pushes 3
    in.numpad[3] = 1;
    // person on floor 2
    in.doorsSensors[2] = 1;
    updateAndCheckState();
    TEST_ASSERT_EQUAL(2, st.nextFloor);
    TEST_ASSERT_EQUAL(1, st.numpad[3]);

    // elv going to floor 2
    st.currentFloor = 1;
    st.isMoving = 1;
    // person on floor 2
    in.doorsSensors[2] = 1;
    updateAndCheckState();
    TEST_ASSERT_EQUAL(2, st.nextFloor);
    TEST_ASSERT_EQUAL(1, st.numpad[3]);

    // elv in floor 2
    st.currentFloor = 2;
    st.isMoving = 0;
    // person goes on pushes 4
    in.numpad[4] = 1;
    updateAndCheckState();
    TEST_ASSERT_EQUAL(3, st.nextFloor);
    TEST_ASSERT_EQUAL(1, st.numpad[3]);
    TEST_ASSERT_EQUAL(1, st.numpad[4]);

    // elv in floor 3
    st.currentFloor = 3;
    updateAndCheckState();
    TEST_ASSERT_EQUAL(4, st.nextFloor);
    TEST_ASSERT_EQUAL(0, st.numpad[3]);
    TEST_ASSERT_EQUAL(1, st.numpad[4]);

    // elv in floor 4
    st.currentFloor = 4;
    updateAndCheckState();
    TEST_ASSERT_EQUAL(4, st.nextFloor);
    TEST_ASSERT_EQUAL(0, st.numpad[3]);
    TEST_ASSERT_EQUAL(0, st.numpad[4]);
}

void test_scenario_12(void) {
    // person at 1
    in.doorsSensors[1] = 1;
    // pushed up
    in.floorUpButtons[1] = 1;
    updateAndCheckState();
    TEST_ASSERT_EQUAL(1, st.nextFloor);

    // elv between 0,1
    st.currentFloor = 0;
    st.isMoving = 1;
    // person at 1
    in.doorsSensors[1] = 1;
    updateAndCheckState();
    TEST_ASSERT_EQUAL(1, st.nextFloor);

    // elv in 1
    st.currentFloor = 1;
    st.isMoving = 0;
    // person run away
    // person at 2
    in.doorsSensors[2] = 1;
    // pushed up
    in.floorUpButtons[2] = 1;
    updateAndCheckState();
    TEST_ASSERT_EQUAL(2, st.nextFloor);

    // elv between 1,2
    st.currentFloor = 1;
    st.isMoving = 1;
    // person at 2
    in.doorsSensors[2] = 1;
    updateAndCheckState();
    TEST_ASSERT_EQUAL(2, st.nextFloor);

    // elv in 2
    st.currentFloor = 2;
    st.isMoving = 0;
    // person run away
    updateAndCheckState();
    TEST_ASSERT_EQUAL(2, st.nextFloor);
}

void test_scenario_13(void) {
    // 2 persons on floor 0
    in.doorsSensors[0] = 1;
    // pushed up
    in.floorUpButtons[0] = 1;
    // walked in, pushed 1,2
    in.numpad[1] = in.numpad[2] = 1;
    updateAndCheckState();
    TEST_ASSERT_EQUAL(1, st.nextFloor);
    TEST_ASSERT_EQUAL(1, st.numpad[1]);
    TEST_ASSERT_EQUAL(1, st.numpad[2]);
    
    // elv in 1
    st.currentFloor = 1;
    // person goes out
    in.doorsSensors[1] = 1;
    updateAndCheckState();
    TEST_ASSERT_EQUAL(2, st.nextFloor);
    TEST_ASSERT_EQUAL(0, st.numpad[1]);
    TEST_ASSERT_EQUAL(1, st.numpad[2]);

    // elv in 2
    st.currentFloor = 2;
    // person goes out
    in.doorsSensors[2] = 1;
    updateAndCheckState();
    TEST_ASSERT_EQUAL(2, st.nextFloor);
    TEST_ASSERT_EQUAL(0, st.numpad[1]);
    TEST_ASSERT_EQUAL(0, st.numpad[2]);
}

void test_scenario_14(void) {
    // elevator at floor 0 
    st.currentFloor = 0;
    // person at floor 4 call the elevator
    in.doorsSensors[4] = 1;
    in.floorUpButtons[4] = 1;

    updateAndCheckState();
    in.floorUpButtons[4] = 0;
    TEST_ASSERT_EQUAL(4, st.nextFloor);
    TEST_ASSERT_EQUAL(1, st.reqUp[4]);
   
    // elv between 1 and 2
    st.currentFloor = 1;
    st.isMoving = true;

    // person at floor 4 left
    in.doorsSensors[4] = 0;
    updateAndCheckState();
    //TEST_ASSERT_EQUAL(2, st.nextFloor);
    TEST_ASSERT_EQUAL(0, st.reqUp[4]);

    // elv in 2
    st.currentFloor = 2;
    st.isMoving = false;
    updateAndCheckState();
    TEST_ASSERT_EQUAL(2, st.nextFloor);
}


int main() {
    UNITY_BEGIN();
    RUN_TEST(test_or_bits);
    RUN_TEST(test_and_bits);
    RUN_TEST(test_getIndexOfNextHighBit);
    RUN_TEST(test_getIndexOfPrevHighBit);

    RUN_TEST(test_zeroed);
    RUN_TEST(test_scenario_0);
    RUN_TEST(test_scenario_1);
    RUN_TEST(test_scenario_2);
    RUN_TEST(test_scenario_3);
    RUN_TEST(test_scenario_4);
    RUN_TEST(test_scenario_5);
    RUN_TEST(test_scenario_6);
    RUN_TEST(test_scenario_7);
    RUN_TEST(test_scenario_8);
    RUN_TEST(test_scenario_9);
    RUN_TEST(test_scenario_10);
    RUN_TEST(test_scenario_11);
    RUN_TEST(test_scenario_12);
    RUN_TEST(test_scenario_13);
    RUN_TEST(test_scenario_14);
    UNITY_END();

    return 0;
}

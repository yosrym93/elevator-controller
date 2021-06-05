#include <unity.h>
#include <nextState.h>

constexpr size_t NUM_FLOORS = 5;
State<NUM_FLOORS> st;
Input<NUM_FLOORS> in;

inline void updateAndCheckState(void) {
    uint8_t _floor = st.currentFloor;
    st = nextState(&in, &st);
    // shouldn't change the currentFloor
    TEST_ASSERT_EQUAL(_floor, st.currentFloor);
}

void setUp(void) {
    st = ZERO_STRUCT;
    in = ZERO_STRUCT;
}

void test_zeroed(void) {
    TEST_ASSERT_EQUAL(0, st.currentFloor);
    TEST_ASSERT_EQUAL(0, st.nextFloor);
    for (auto i : st.floorDownButtons) {
        TEST_ASSERT_EQUAL(0, i);
    }
    for (auto i : st.floorUpButtons) {
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
    TEST_ASSERT_EQUAL(1, st.floorUpButtons[0]);

    // person goes on elv
    in.floorUpButtons[0] = in.doorsSensors[0] = 0;
    // pushes 1
    in.numpad[1] = 1;
    updateAndCheckState();
    TEST_ASSERT_EQUAL(1, st.nextFloor);
    TEST_ASSERT_EQUAL(0, st.floorUpButtons[0]);
    TEST_ASSERT_EQUAL(1, st.numpad[1]);

    // elv reaches floor 1
    st.currentFloor = 1;
    updateAndCheckState();
    TEST_ASSERT_EQUAL(1, st.nextFloor);
    TEST_ASSERT_EQUAL(0, st.floorUpButtons[0]);
    TEST_ASSERT_EQUAL(0, st.numpad[1]);
}

void test_scenario_1(void) {
    // 2 persons on floor 0
    in.doorsSensors[0] = 1;
    // push up
    in.floorUpButtons[0] = 1;
    updateAndCheckState();
    TEST_ASSERT_EQUAL(0, st.nextFloor);
    TEST_ASSERT_EQUAL(0, st.floorUpButtons[0]);

    // 1 person goes on elv
    in.floorUpButtons[0] = 0;
    // the other was still standing
    in.doorsSensors[0] = 1;
    // pushes 2
    in.numpad[2] = 1;
    updateAndCheckState();
    TEST_ASSERT_EQUAL(2, st.nextFloor);
    TEST_ASSERT_EQUAL(0, st.floorUpButtons[0]);
    TEST_ASSERT_EQUAL(1, st.numpad[2]);

    // elv reaches floor 1
    st.currentFloor = 1;
    updateAndCheckState();
    TEST_ASSERT_EQUAL(2, st.nextFloor);
    TEST_ASSERT_EQUAL(0, st.floorUpButtons[0]);
    TEST_ASSERT_EQUAL(1, st.numpad[2]);

    // elv reaches floor 2
    st.currentFloor = 2;
    updateAndCheckState();
    TEST_ASSERT_EQUAL(2, st.nextFloor);
    TEST_ASSERT_EQUAL(0, st.floorUpButtons[0]);
    TEST_ASSERT_EQUAL(0, st.numpad[2]);
}

void test_scenario_2(void) {
    // 1 person on floor 1
    in.doorsSensors[1] = 1;
    // push down
    in.floorDownButtons[1] = 1;
    updateAndCheckState();
    TEST_ASSERT_EQUAL(1, st.nextFloor);
    TEST_ASSERT_EQUAL(1, st.floorUpButtons[1]);

    // elv reached floor 1
    st.currentFloor = 1;
    // 1 person stepped in
    in.doorsSensors[1] = 0;
    in.floorDownButtons[1] = 0;
    // pushes 3
    in.numpad[3] = 1;
    updateAndCheckState();
    TEST_ASSERT_EQUAL(3, st.nextFloor);
    TEST_ASSERT_EQUAL(0, st.floorDownButtons[1]);
    TEST_ASSERT_EQUAL(1, st.numpad[3]);

    // elv reaches floor 3
    st.currentFloor = 3;
    updateAndCheckState();
    TEST_ASSERT_EQUAL(3, st.nextFloor);
    TEST_ASSERT_EQUAL(0, st.floorDownButtons[1]);
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
    TEST_ASSERT_EQUAL(1, st.floorDownButtons[1]);

    // elv on floor 1
    st.currentFloor = 1;
    // person goes on elv
    in.doorsSensors[1] = 0;
    in.floorDownButtons[1] = 0;
    // pushes 0
    in.numpad[0] = 1;
    updateAndCheckState();
    TEST_ASSERT_EQUAL(0, st.nextFloor);
    TEST_ASSERT_EQUAL(0, st.floorDownButtons[1]);
    TEST_ASSERT_EQUAL(1, st.numpad[0]);

    // elv reaches floor 0
    st.currentFloor = 0;
    updateAndCheckState();
    TEST_ASSERT_EQUAL(0, st.nextFloor);
    TEST_ASSERT_EQUAL(0, st.floorDownButtons[1]);
    TEST_ASSERT_EQUAL(0, st.numpad[0]);
}

void test_scenario_4(void) {
    // elv on floor 4
    st.currentFloor = 4;
    // 1 person on floor 1
    in.doorsSensors[1] = 1;
    // push down
    in.floorDownButtons[1] = 1;
    updateAndCheckState();
    TEST_ASSERT_EQUAL(1, st.nextFloor);
    TEST_ASSERT_EQUAL(1, st.floorDownButtons[1]);

    // elv on floor 3
    st.currentFloor = 3;
    // person on floor 2
    in.doorsSensors[2] = 1;
    // pushes up
    in.floorUpButtons[2] = 1;
    updateAndCheckState();
    TEST_ASSERT_EQUAL(1, st.nextFloor);
    TEST_ASSERT_EQUAL(1, st.floorUpButtons[2]);
    TEST_ASSERT_EQUAL(1, st.floorDownButtons[1]);

    // elv on floor 1
    st.currentFloor = 1;
    // person goes on elv
    in.doorsSensors[1] = 0;
    in.floorDownButtons[1] = 0;
    // pushes 2
    in.numpad[2] = 1;
    updateAndCheckState();
    TEST_ASSERT_EQUAL(2, st.nextFloor);
    TEST_ASSERT_EQUAL(1, st.floorUpButtons[2]);
    TEST_ASSERT_EQUAL(0, st.floorDownButtons[1]);
    TEST_ASSERT_EQUAL(1, st.numpad[2]);

    // elv reaches floor 2
    st.currentFloor = 2;
    // one goes in another goes out
    in.floorUpButtons[2] = 0;
    // pushes 4
    in.numpad[4] = 1;
    updateAndCheckState();
    TEST_ASSERT_EQUAL(4, st.nextFloor);
    TEST_ASSERT_EQUAL(0, st.floorUpButtons[2]);
    TEST_ASSERT_EQUAL(0, st.floorDownButtons[1]);
    TEST_ASSERT_EQUAL(0, st.numpad[2]);
    TEST_ASSERT_EQUAL(1, st.numpad[4]);

    // elv reaches floor 4
    st.currentFloor = 4;
    // one goes out
    in.doorsSensors[4] = 1;
    updateAndCheckState();
    TEST_ASSERT_EQUAL(4, st.nextFloor);
    TEST_ASSERT_EQUAL(0, st.floorUpButtons[2]);
    TEST_ASSERT_EQUAL(0, st.floorDownButtons[1]);
    TEST_ASSERT_EQUAL(0, st.numpad[2]);
    TEST_ASSERT_EQUAL(0, st.numpad[4]);
}

void test_scenario_5(void) {
    // 1 person on floor 1
    st.currentFloor = 4;
    // elv on floor 4
    in.doorsSensors[1] = 1;
    // push down
    in.floorDownButtons[1] = 1;
    updateAndCheckState();
    TEST_ASSERT_EQUAL(1, st.nextFloor);
    TEST_ASSERT_EQUAL(1, st.floorDownButtons[1]);

    // elv on floor 3
    st.currentFloor = 3;
    // person on floor 2
    in.doorsSensors[2] = 1;
    // pushes down
    in.floorDownButtons[2] = 1;
    updateAndCheckState();
    TEST_ASSERT_EQUAL(2, st.nextFloor);
    TEST_ASSERT_EQUAL(1, st.floorDownButtons[2]);
    TEST_ASSERT_EQUAL(1, st.floorDownButtons[1]);

    // elv on floor 2
    st.currentFloor = 2;
    // person goes on elv
    in.doorsSensors[2] = 0;
    in.floorDownButtons[2] = 0;
    // pushes 0
    in.numpad[0] = 1;
    updateAndCheckState();
    TEST_ASSERT_EQUAL(1, st.nextFloor);
    TEST_ASSERT_EQUAL(0, st.floorDownButtons[2]);
    TEST_ASSERT_EQUAL(1, st.floorDownButtons[1]);
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
    TEST_ASSERT_EQUAL(0, st.floorDownButtons[2]);
    TEST_ASSERT_EQUAL(0, st.floorDownButtons[1]);
    TEST_ASSERT_EQUAL(1, st.numpad[0]);

    // elv reaches floor 1
    st.currentFloor = 1;
    // two go out
    in.doorsSensors[0] = 1;
    updateAndCheckState();
    TEST_ASSERT_EQUAL(0, st.nextFloor);
    TEST_ASSERT_EQUAL(0, st.floorDownButtons[2]);
    TEST_ASSERT_EQUAL(0, st.floorDownButtons[1]);
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
    TEST_ASSERT_EQUAL(1, st.floorUpButtons[1]);

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
    TEST_ASSERT_EQUAL(1, st.floorUpButtons[2]);
    TEST_ASSERT_EQUAL(0, st.floorUpButtons[1]);
    TEST_ASSERT_EQUAL(1, in.numpad[4]);

    // elv on floor 2
    st.currentFloor = 2;
    // person goes on elv
    in.doorsSensors[2] = 0;
    in.floorUpButtons[2] = 0;
    // pushes 4
    in.numpad[4] = 1;
    updateAndCheckState();
    TEST_ASSERT_EQUAL(4, st.nextFloor);
    TEST_ASSERT_EQUAL(0, st.floorUpButtons[2]);
    TEST_ASSERT_EQUAL(0, st.floorUpButtons[1]);
    TEST_ASSERT_EQUAL(1, st.numpad[4]);

    // elv on floor 4
    st.currentFloor = 4;
    // 2 persons go out
    in.doorsSensors[4] = 1;
    updateAndCheckState();
    TEST_ASSERT_EQUAL(4, st.nextFloor);
    TEST_ASSERT_EQUAL(0, st.floorUpButtons[2]);
    TEST_ASSERT_EQUAL(0, st.floorUpButtons[1]);
    TEST_ASSERT_EQUAL(0, st.numpad[4]);
}

int main() {
    UNITY_BEGIN();
    RUN_TEST(test_zeroed);
    RUN_TEST(test_scenario_0);
    RUN_TEST(test_scenario_1);
    RUN_TEST(test_scenario_2);
    RUN_TEST(test_scenario_3);
    RUN_TEST(test_scenario_4);
    RUN_TEST(test_scenario_5);
    RUN_TEST(test_scenario_6);
    UNITY_END();

    return 0;
}

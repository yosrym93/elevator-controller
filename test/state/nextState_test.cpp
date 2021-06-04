#include <unity.h>
#include <nextState.h>

constexpr size_t NUM_FLOORS = 5;
State<NUM_FLOORS> st;
Input<NUM_FLOORS> in;

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
    // 1 person on floor 0, push up
    in.floorUpButtons[0] = in.doorsSensors[0] = 1;
    st = nextState(&in, &st);
    TEST_ASSERT_EQUAL(0, st.currentFloor);
    TEST_ASSERT_EQUAL(0, st.nextFloor);
    TEST_ASSERT_EQUAL(1, st.floorUpButtons[0]);

    // person goes on elevator, pushes 1
    in.floorUpButtons[0] = in.doorsSensors[0] = 0;
    in.numpad[1] = 1;
    st = nextState(&in, &st);
    TEST_ASSERT_EQUAL(0, st.currentFloor);
    TEST_ASSERT_EQUAL(1, st.nextFloor);
    TEST_ASSERT_EQUAL(0, st.floorUpButtons[0]);
    TEST_ASSERT_EQUAL(1, st.numpad[1]);

    // elevator reaches floor 1
    st.currentFloor = 1;
    st = nextState(&in, &st);
    TEST_ASSERT_EQUAL(1, st.currentFloor);
    TEST_ASSERT_EQUAL(1, st.nextFloor);
    TEST_ASSERT_EQUAL(0, st.floorUpButtons[0]);
    TEST_ASSERT_EQUAL(0, st.numpad[1]);
}

void test_scenario_1(void) {
    // 2 persons on floor 0, push up
    in.floorUpButtons[0] = in.doorsSensors[0] = 1;
    st = nextState(&in, &st);
    TEST_ASSERT_EQUAL(0, st.currentFloor);
    TEST_ASSERT_EQUAL(0, st.nextFloor);
    TEST_ASSERT_EQUAL(0, st.floorUpButtons[0]);

    // 1 person goes on elevator, the other was still standing, pushes 2
    in.floorUpButtons[0] = 0;
    in.doorsSensors[0] = 1;
    in.numpad[2] = 1;
    st = nextState(&in, &st);
    TEST_ASSERT_EQUAL(0, st.currentFloor);
    TEST_ASSERT_EQUAL(2, st.nextFloor);
    TEST_ASSERT_EQUAL(0, st.floorUpButtons[0]);
    TEST_ASSERT_EQUAL(1, st.numpad[2]);

    // elevator reaches floor 1
    st.currentFloor = 1;
    st = nextState(&in, &st);
    TEST_ASSERT_EQUAL(1, st.currentFloor);
    TEST_ASSERT_EQUAL(2, st.nextFloor);
    TEST_ASSERT_EQUAL(0, st.floorUpButtons[0]);
    TEST_ASSERT_EQUAL(1, st.numpad[2]);

    // elevator reaches floor 2
    st.currentFloor = 2;
    st = nextState(&in, &st);
    TEST_ASSERT_EQUAL(2, st.currentFloor);
    TEST_ASSERT_EQUAL(2, st.nextFloor);
    TEST_ASSERT_EQUAL(0, st.floorUpButtons[0]);
    TEST_ASSERT_EQUAL(0, st.numpad[2]);
}

void test_scenario_2(void) {
    // 1 person on floor 1, push down
    in.floorDownButtons[1] = in.doorsSensors[1] = 1;
    st = nextState(&in, &st);
    TEST_ASSERT_EQUAL(0, st.currentFloor);
    TEST_ASSERT_EQUAL(1, st.nextFloor);
    TEST_ASSERT_EQUAL(1, st.floorUpButtons[1]);

    // elevator reached floor 1, 1 person stepped in, pushes 3
    st.currentFloor = 1;
    in.floorDownButtons[1] = in.doorsSensors[1] = 0;
    in.numpad[3] = 1;
    st = nextState(&in, &st);
    TEST_ASSERT_EQUAL(1, st.currentFloor);
    TEST_ASSERT_EQUAL(3, st.nextFloor);
    TEST_ASSERT_EQUAL(0, st.floorUpButtons[1]);
    TEST_ASSERT_EQUAL(1, st.numpad[3]);

    // elevator reaches floor 3
    st.currentFloor = 3;
    st = nextState(&in, &st);
    TEST_ASSERT_EQUAL(3, st.currentFloor);
    TEST_ASSERT_EQUAL(3, st.nextFloor);
    TEST_ASSERT_EQUAL(0, st.floorUpButtons[1]);
    TEST_ASSERT_EQUAL(0, st.numpad[3]);
}

void test_scenario_3(void) {
    // elv on floor 4, 1 person on floor 1, push down
    st.currentFloor = 4;
    in.floorDownButtons[1] = in.doorsSensors[1] = 1;
    st = nextState(&in, &st);
    TEST_ASSERT_EQUAL(1, st.currentFloor);
    TEST_ASSERT_EQUAL(1, st.nextFloor);
    TEST_ASSERT_EQUAL(1, st.floorDownButtons[1]);

    // elv on floor 1, person goes on elevator, pushes 0
    st.currentFloor = 1;
    in.floorDownButtons[1] = in.doorsSensors[1] = 0;
    in.numpad[0] = 1;
    st = nextState(&in, &st);
    TEST_ASSERT_EQUAL(1, st.currentFloor);
    TEST_ASSERT_EQUAL(0, st.nextFloor);
    TEST_ASSERT_EQUAL(0, st.floorDownButtons[1]);
    TEST_ASSERT_EQUAL(1, st.numpad[0]);

    // elevator reaches floor 0
    st.currentFloor = 0;
    st = nextState(&in, &st);
    TEST_ASSERT_EQUAL(0, st.currentFloor);
    TEST_ASSERT_EQUAL(0, st.nextFloor);
    TEST_ASSERT_EQUAL(0, st.floorDownButtons[1]);
    TEST_ASSERT_EQUAL(0, st.numpad[0]);
}

void test_scenario_4(void) {
    // elv on floor 4, 1 person on floor 1, push down
    st.currentFloor = 4;
    in.floorDownButtons[1] = in.doorsSensors[1] = 1;
    st = nextState(&in, &st);
    TEST_ASSERT_EQUAL(1, st.currentFloor);
    TEST_ASSERT_EQUAL(1, st.nextFloor);
    TEST_ASSERT_EQUAL(1, st.floorDownButtons[1]);

    // elv on floor 3, person on floor 2, pushes up
    st.currentFloor = 3;
    in.floorUpButtons[2] = in.doorsSensors[2] = 1;
    st = nextState(&in, &st);
    TEST_ASSERT_EQUAL(3, st.currentFloor);
    TEST_ASSERT_EQUAL(1, st.nextFloor);
    TEST_ASSERT_EQUAL(1, st.floorUpButtons[2]);
    TEST_ASSERT_EQUAL(1, st.floorDownButtons[1]);

    // elv on floor 1, person goes on elevator, pushes 2
    st.currentFloor = 1;
    in.floorDownButtons[1] = in.doorsSensors[1] = 0;
    in.numpad[2] = 1;
    st = nextState(&in, &st);
    TEST_ASSERT_EQUAL(1, st.currentFloor);
    TEST_ASSERT_EQUAL(2, st.nextFloor);
    TEST_ASSERT_EQUAL(1, st.floorUpButtons[2]);
    TEST_ASSERT_EQUAL(0, st.floorDownButtons[1]);
    TEST_ASSERT_EQUAL(1, st.numpad[2]);

    // elevator reaches floor 2, one goes in another goes out, pushes 4
    st.currentFloor = 2;
    in.floorUpButtons[2] = 0;
    in.numpad[4] = 1;
    st = nextState(&in, &st);
    TEST_ASSERT_EQUAL(2, st.currentFloor);
    TEST_ASSERT_EQUAL(4, st.nextFloor);
    TEST_ASSERT_EQUAL(0, st.floorUpButtons[2]);
    TEST_ASSERT_EQUAL(0, st.floorDownButtons[1]);
    TEST_ASSERT_EQUAL(0, st.numpad[2]);
    TEST_ASSERT_EQUAL(1, st.numpad[4]);

    // elv reaches floor 4, one goes out
    st.currentFloor = 4;
    st = nextState(&in, &st);
    TEST_ASSERT_EQUAL(4, st.currentFloor);
    TEST_ASSERT_EQUAL(4, st.nextFloor);
    TEST_ASSERT_EQUAL(0, st.floorUpButtons[2]);
    TEST_ASSERT_EQUAL(0, st.floorDownButtons[1]);
    TEST_ASSERT_EQUAL(0, st.numpad[2]);
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
    UNITY_END();

    return 0;
}

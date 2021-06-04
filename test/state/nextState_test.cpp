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
    TEST_ASSERT_EQUAL(0, st.currentLevel);
    TEST_ASSERT_EQUAL(0, st.nextLevel);
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
    // if: 1 person on floor 0, push up
    in.floorUpButtons[0] = in.doorsSensors[0] = 1;
    st = nextState(&in, &st);
    // then: floor 0 is next
    TEST_ASSERT_EQUAL(0, st.currentLevel);
    TEST_ASSERT_EQUAL(0, st.nextLevel);
    // then: floorUpButton[0] is off
    TEST_ASSERT_EQUAL(1, st.floorUpButtons[0]);

    // if: person goes on elevator, pushes 1
    in.floorUpButtons[0] = in.doorsSensors[0] = 0;
    in.numpad[1] = 1;
    st = nextState(&in, &st);
    // then: floor 1 is next
    TEST_ASSERT_EQUAL(0, st.currentLevel);
    TEST_ASSERT_EQUAL(1, st.nextLevel);
    // then: floorUpButton[0] is off
    TEST_ASSERT_EQUAL(0, st.floorUpButtons[0]);
    // then: numpad[1] is on
    TEST_ASSERT_EQUAL(1, st.numpad[1]);

    // if: elevator reaches floor 1
    st.currentLevel = 1;
    st = nextState(&in, &st);
    // then: elevator stops
    TEST_ASSERT_EQUAL(1, st.currentLevel);
    TEST_ASSERT_EQUAL(1, st.nextLevel);
    // then: floorUpButton[0] is off
    TEST_ASSERT_EQUAL(0, st.floorUpButtons[0]);
    // then: numpad[1] is off
    TEST_ASSERT_EQUAL(0, st.numpad[1]);
}

void test_scenario_1(void) {
    // if: 2 persons on floor 0, push up
    in.floorUpButtons[0] = in.doorsSensors[0] = 1;
    st = nextState(&in, &st);
    // then: floor 0 is next
    TEST_ASSERT_EQUAL(0, st.currentLevel);
    TEST_ASSERT_EQUAL(0, st.nextLevel);
    // then: floorUpButton[0] is off
    TEST_ASSERT_EQUAL(0, st.floorUpButtons[0]);

    // if: 1 person goes on elevator, the other was still standing, pushes 2
    in.floorUpButtons[0] = 0;
    in.doorsSensors[0] = 1;
    in.numpad[2] = 1;
    st = nextState(&in, &st);
    // then: floor 2 is next
    TEST_ASSERT_EQUAL(0, st.currentLevel);
    TEST_ASSERT_EQUAL(2, st.nextLevel);
    // then: floorUpButton[0] is off
    TEST_ASSERT_EQUAL(0, st.floorUpButtons[0]);
    // then: numpad[2] is on
    TEST_ASSERT_EQUAL(1, st.numpad[2]);

    // if: elevator reaches floor 1
    st.currentLevel = 1;
    st = nextState(&in, &st);
    // then: elevator continues to floor 2
    TEST_ASSERT_EQUAL(1, st.currentLevel);
    TEST_ASSERT_EQUAL(2, st.nextLevel);
    // then: floorUpButton[0] is off
    TEST_ASSERT_EQUAL(0, st.floorUpButtons[0]);
    // then: numpad[2] is on
    TEST_ASSERT_EQUAL(1, st.numpad[2]);

    // if: elevator reaches floor 2
    st.currentLevel = 2;
    st = nextState(&in, &st);
    // then: elevator stops
    TEST_ASSERT_EQUAL(2, st.currentLevel);
    TEST_ASSERT_EQUAL(2, st.nextLevel);
    // then: floorUpButton[0] is off
    TEST_ASSERT_EQUAL(0, st.floorUpButtons[0]);
    // then: numpad[2] is off
    TEST_ASSERT_EQUAL(0, st.numpad[2]);
}

void test_scenario_2(void) {
    // if: 1 person on floor 1, push down
    in.floorDownButtons[1] = in.doorsSensors[1] = 1;
    st = nextState(&in, &st);
    // then: floor 1 is next
    TEST_ASSERT_EQUAL(0, st.currentLevel);
    TEST_ASSERT_EQUAL(1, st.nextLevel);
    // then: keep floorUpButton[1]
    TEST_ASSERT_EQUAL(1, st.floorUpButtons[1]);

    // if: elevator reached floor 1, 1 person stepped in, pushes 3
    st.currentLevel = 1;
    in.floorDownButtons[1] = in.doorsSensors[1] = 0;
    in.numpad[3] = 1;
    st = nextState(&in, &st);
    // then: floor 3 is next
    TEST_ASSERT_EQUAL(1, st.currentLevel);
    TEST_ASSERT_EQUAL(3, st.nextLevel);
    // then: floorUpButton[1] is off
    TEST_ASSERT_EQUAL(0, st.floorUpButtons[1]);
    // then: numpad[3] is on
    TEST_ASSERT_EQUAL(1, st.numpad[3]);

    // if: elevator reaches floor 3
    st.currentLevel = 3;
    st = nextState(&in, &st);
    // then: elevator stops
    TEST_ASSERT_EQUAL(3, st.currentLevel);
    TEST_ASSERT_EQUAL(3, st.nextLevel);
    // then: floorUpButton[1] is off
    TEST_ASSERT_EQUAL(0, st.floorUpButtons[1]);
    // then: numpad[3] is off
    TEST_ASSERT_EQUAL(0, st.numpad[3]);
}

int main() {
    UNITY_BEGIN();
    RUN_TEST(test_zeroed);
    RUN_TEST(test_scenario_0);
    RUN_TEST(test_scenario_1);
    RUN_TEST(test_scenario_2);
    UNITY_END();

    return 0;
}

#include <unity.h>
#include <nextState.h>

constexpr size_t LEVELS = 5;
State<LEVELS> curr, next;
Input<LEVELS> in;

void setUp(void) {
    curr = ZERO_STRUCT;
    next = ZERO_STRUCT;
    in = ZERO_STRUCT;
}

void test_zeroed(void) {
    TEST_ASSERT_EQUAL(0, curr.currentLevel);
    TEST_ASSERT_EQUAL(0, curr.nextLevel);
    for (auto i : curr.floorDownButtons) {
        TEST_ASSERT_EQUAL(0, i);
    }
    for (auto i : curr.floorUpButtons) {
        TEST_ASSERT_EQUAL(0, i);
    }
    for (auto i : curr.numpad) {
        TEST_ASSERT_EQUAL(0, i);
    }
}

void test_scenario_0(void) {
    // if: 1 person on level 0, push up
    in.floorUpButtons[0] = in.doorsSensors[0] = 1;
    nextState(&in, &curr, &next);
    // then: level 1 is next
    TEST_ASSERT_EQUAL(0, next.currentLevel);
    TEST_ASSERT_EQUAL(1, next.nextLevel);
    // then: keep floorUpButton[0]
    TEST_ASSERT_EQUAL(1, next.floorUpButtons[0]);

    // if: person goes on elevator
    in.floorUpButtons[0] = in.doorsSensors[0] = 0;
    nextState(&in, &curr, &next);
    // then: level 1 is next
    TEST_ASSERT_EQUAL(0, next.currentLevel);
    TEST_ASSERT_EQUAL(1, next.nextLevel);
    // then: floorUpButton[0] is off
    TEST_ASSERT_EQUAL(0, next.floorUpButtons[0]);

    // if: elevator reaches level 1
    curr.currentLevel = 1;
    nextState(&in, &curr, &next);
    // then: elevator stops
    TEST_ASSERT_EQUAL(1, next.currentLevel);
    TEST_ASSERT_EQUAL(1, next.nextLevel);
    // then: floorUpButton[0] is off
    TEST_ASSERT_EQUAL(0, next.floorUpButtons[0]);
}

void test_scenario_1(void) {
    // if: 2 persons on level 0, push up
    in.floorUpButtons[0] = in.doorsSensors[0] = 1;
    nextState(&in, &curr, &next);
    // then: level 1 is next
    TEST_ASSERT_EQUAL(0, next.currentLevel);
    TEST_ASSERT_EQUAL(1, next.nextLevel);
    // then: keep floorUpButton[0]
    TEST_ASSERT_EQUAL(1, next.floorUpButtons[0]);

    // if: 1 person goes on elevator, the other was still standing
    in.floorUpButtons[0] = 0;
    in.doorsSensors[0] = 1;
    nextState(&in, &curr, &next);
    // then: level 1 is next
    TEST_ASSERT_EQUAL(0, next.currentLevel);
    TEST_ASSERT_EQUAL(1, next.nextLevel);
    // then: floorUpButton[0] is off
    TEST_ASSERT_EQUAL(0, next.floorUpButtons[0]);

    // if: elevator reaches level 1
    curr.currentLevel = 1;
    nextState(&in, &curr, &next);
    // then: elevator stops
    TEST_ASSERT_EQUAL(1, next.currentLevel);
    TEST_ASSERT_EQUAL(1, next.nextLevel);
    // then: floorUpButton[0] is off
    TEST_ASSERT_EQUAL(0, next.floorUpButtons[0]);
}

void test_scenario_2(void) {
    // if: 1 person on level 1, push down
    in.floorDownButtons[1] = in.doorsSensors[1] = 1;
    nextState(&in, &curr, &next);
    // then: level 1 is next
    TEST_ASSERT_EQUAL(0, next.currentLevel);
    TEST_ASSERT_EQUAL(1, next.nextLevel);
    // then: keep floorUpButton[1]
    TEST_ASSERT_EQUAL(1, next.floorUpButtons[1]);

    // if: elevator reached level 1, 1 person stepped in
    curr.currentLevel = 1;
    in.floorDownButtons[1] = in.doorsSensors[1] = 0;
    nextState(&in, &curr, &next);
    // then: level 0 is next
    TEST_ASSERT_EQUAL(1, next.currentLevel);
    TEST_ASSERT_EQUAL(0, next.nextLevel);
    // then: floorUpButton[1] is off
    TEST_ASSERT_EQUAL(0, next.floorUpButtons[1]);

    // if: elevator reaches level 0
    curr.currentLevel = 0;
    nextState(&in, &curr, &next);
    // then: elevator stops
    TEST_ASSERT_EQUAL(0, next.currentLevel);
    TEST_ASSERT_EQUAL(0, next.nextLevel);
    // then: floorUpButton[1] is off
    TEST_ASSERT_EQUAL(0, next.floorUpButtons[1]);
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
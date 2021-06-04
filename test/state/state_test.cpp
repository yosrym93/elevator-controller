#include <unity.h>
#include <nextState.h>

void test_zeroed() {
    State<5> s = ZERO_STRUCT;
    TEST_ASSERT_EQUAL(0, s.currentLevel);
    TEST_ASSERT_EQUAL(0, s.nextLevel);
    for (auto i : s.floorDownButtons) {
        TEST_ASSERT_EQUAL(0, i);
    }
    for (auto i : s.floorUpButtons) {
        TEST_ASSERT_EQUAL(0, i);
    }
    for (auto i : s.numpad) {
        TEST_ASSERT_EQUAL(0, i);
    }
}

int main() {
    UNITY_BEGIN();
    RUN_TEST(test_zeroed);
    UNITY_END();

    return 0;
}

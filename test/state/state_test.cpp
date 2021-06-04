#include <unity.h>

void test_example() {
    TEST_ASSERT_EQUAL(1, 1); // TODO: remove
}

int main() {
    UNITY_BEGIN();
    RUN_TEST(test_example);
    UNITY_END();

    return 0;
}

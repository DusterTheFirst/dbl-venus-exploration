#include <unity.h>

#include "test.hpp"

COBS_TEST_GLOBALS

extern void test9(void);
extern void test10(void);
extern void test11(void);

void setup(void) {
    UNITY_BEGIN();

    RUN_TEST(test9);
    RUN_TEST(test10);
    RUN_TEST(test11);

    UNITY_END();
}

void loop() {
    UNITY_END();
}

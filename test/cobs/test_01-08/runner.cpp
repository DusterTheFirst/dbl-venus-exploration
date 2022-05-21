#include <unity.h>

#include "test.hpp"

COBS_TEST_GLOBALS

extern void test1(void);
extern void test2(void);
extern void test3(void);
extern void test4(void);
extern void test5(void);
extern void test6(void);
extern void test7(void);
extern void test8(void);

void setup(void) {
    UNITY_BEGIN();

    RUN_TEST(test1);
    RUN_TEST(test2);
    RUN_TEST(test3);
    RUN_TEST(test4);
    RUN_TEST(test5);
    RUN_TEST(test6);
    RUN_TEST(test7);
    RUN_TEST(test8);

    UNITY_END();
}

void loop() {
    UNITY_END();
}

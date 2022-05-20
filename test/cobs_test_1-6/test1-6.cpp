#include "../cobs_test.hpp"

static void test1() {
    uint8_t input[] = {
        0x00,

        0xDE, 0xAD, 0xBE, 0xEF
    };
    uint8_t output[] = { 0x01, 0x01, 0x00 };

    serial_test(input, output);
}

static void test2() {
    uint8_t input[] = {
        0x00, 0x00,

        0xDE, 0xAD, 0xBE, 0xEF
    };
    uint8_t output[] = { 0x01, 0x01, 0x01, 0x00 };

    serial_test(input, output);
}

static void test3(void) {
    uint8_t input[] = {
        0x00, 0x11, 0x00,

        0xDE, 0xAD, 0xBE, 0xEF
    };
    uint8_t output[] = { 0x01, 0x02, 0x11, 0x01, 0x00 };

    serial_test(input, output);
}

static void test4(void) {
    uint8_t input[] = {
        0x11, 0x22, 0x00, 0x33,

        0xDE, 0xAD, 0xBE, 0xEF
    };
    uint8_t output[] = { 0x03, 0x11, 0x22, 0x02, 0x33, 0x00 };

    serial_test(input, output);
}

static void test5(void) {
    uint8_t input[] = {
        0x11, 0x22, 0x33, 0x44,

        0xDE, 0xAD, 0xBE, 0xEF
    };
    uint8_t output[] = { 0x05, 0x11, 0x22, 0x33, 0x44, 0x00 };

    serial_test(input, output);
}

static void test6(void) {
    uint8_t input[] = {
        0x11, 0x00, 0x00, 0x00,

        0xDE, 0xAD, 0xBE, 0xEF
    };
    uint8_t output[] = { 0x02, 0x11, 0x01, 0x01, 0x01, 0x00 };

    serial_test(input, output);
}

void setup(void) {
    UNITY_BEGIN();

    RUN_TEST(test1);
    RUN_TEST(test2);
    RUN_TEST(test3);
    RUN_TEST(test4);
    RUN_TEST(test5);
    RUN_TEST(test6);

    UNITY_END();
}

void loop() {
    UNITY_END();
}

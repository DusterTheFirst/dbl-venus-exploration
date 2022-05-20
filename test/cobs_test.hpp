#pragma once

#include <Arduino.h>
#include <stdint.h>
#include <string.h>
#include <unity.h>

#ifndef UNIT_TEST
#define UNIT_TEST
#endif

#include "cobs.hpp"

size_t telemetry::cobs::test::byte_offset = 0;
uint8_t *telemetry::cobs::test::output = NULL;

template <size_t INPUT_LEN, size_t OUTPUT_LEN>
static inline void serial_test(uint8_t (&input)[INPUT_LEN],
                               uint8_t (&output)[OUTPUT_LEN]) {
    uint8_t output_buffer[OUTPUT_LEN];

    telemetry::cobs::test::byte_offset = 0;
    telemetry::cobs::test::output = output_buffer;

    telemetry::cobs::serial_write(input, INPUT_LEN - 4);

    TEST_ASSERT_EQUAL_size_t_MESSAGE(
        OUTPUT_LEN,
        telemetry::cobs::test::byte_offset,
        "did not write the correct amount of bytes");

    TEST_ASSERT_EQUAL_UINT8_ARRAY(
        output,
        telemetry::cobs::test::output,
        OUTPUT_LEN);
}

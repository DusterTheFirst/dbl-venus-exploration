#include <Arduino.h>

#include <string.h>

#include "cobs.hpp"

static inline void serial_write_byte(uint8_t byte) {
#ifdef UNIT_TEST
    telemetry::cobs::test::output[telemetry::cobs::test::byte_offset] = byte;
    telemetry::cobs::test::byte_offset++;
#else
    Serial.write(byte);
#endif
}

void telemetry::cobs::serial_write(const uint8_t *data, const size_t size) {
    if (data == NULL)
        return;

    const uint8_t *data_ptr = data;
    const uint8_t *end_ptr = data + size;

    while (data_ptr <= end_ptr) {
        // Distance to the next null byte or the end of the data
        size_t distance = strnlen((const char *)data_ptr, end_ptr - data_ptr);

        // Ensure the distance is clamped to 0xFF
        if (distance > 0xFE) {
            distance = 0xFE;
        }

        // Write the distance to the next null byte
        serial_write_byte(distance + 1);
        // Write the non-null bytes
        for (size_t i = 0; i < distance; i++) {
            serial_write_byte(*data_ptr);
            data_ptr++;
        }

        // Skip the next byte if the next null byte is < 0xFE bytes away
        // or if this is the last byte
        if (distance != 0xFE || data_ptr == end_ptr) {
            data_ptr++;
        }
    }

    serial_write_byte(0);
}

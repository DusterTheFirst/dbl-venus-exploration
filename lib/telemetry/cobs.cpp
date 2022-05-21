#include <Arduino.h>

#include <stdint.h>
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

void telemetry::cobs::serial_write(const chunk::Chunks data) {
    size_t size = data.len();
    for (size_t idx = 0; idx <= size;) {
        // Distance to the next null byte or the end of the data
        size_t distance = 0;
        for (; distance + idx < size; distance++) {
            if (data.get_at(idx + distance) == 0x00) {
                break;
            }
        }

        // Ensure the distance is clamped to 0xFF
        if (distance > 0xFE) {
            distance = 0xFE;
        }

        // Write the distance to the next null byte
        serial_write_byte(distance + 1);
        // Write the non-null bytes
        for (size_t i = 0; i < distance; i++) {
            serial_write_byte(data.get_at(idx));
            idx++;
        }

        // Skip the next byte if the next null byte is < 0xFE bytes away
        // or if this is the last byte
        if (distance != 0xFE || idx == size) {
            idx++;
        }
    }

    serial_write_byte(0);
}

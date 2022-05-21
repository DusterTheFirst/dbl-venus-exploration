#include <unity.h>
#include "test.hpp"

void test1() {
    static const uint8_t chunk1[] = { 0x00 };
    static const chunk::Chunk chunks[] = {
        chunk::Chunk(false, chunk1)
    };

    static const uint8_t output[] = { 0x01, 0x01, 0x00 };

    serial_test(chunk::Chunks(chunks), output);
}

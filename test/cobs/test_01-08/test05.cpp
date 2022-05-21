#include "test.hpp"

void test5(void) {
    static const uint8_t chunk1[] = { 0x11, 0x22, 0x33, 0x44 };
    static const chunk::Chunk chunks[] = {
        chunk::Chunk(false, chunk1)
    };

    static const uint8_t output[] = { 0x05, 0x11, 0x22, 0x33, 0x44, 0x00 };

    serial_test(chunk::Chunks(chunks), output);
}

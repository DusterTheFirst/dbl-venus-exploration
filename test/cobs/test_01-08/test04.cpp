#include "test.hpp"

void test4(void) {
    static const uint8_t chunk1[] = { 0x11, 0x22, 0x00, 0x33 };
    static const chunk::Chunk chunks[] = {
        chunk::Chunk(false, chunk1)
    };

    static const uint8_t output[] = { 0x03, 0x11, 0x22, 0x02, 0x33, 0x00 };

    serial_test(chunk::Chunks(chunks), output);
}

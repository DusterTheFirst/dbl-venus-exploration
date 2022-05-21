#include "test.hpp"

void test6(void) {
    static const uint8_t chunk1[] = { 0x11, 0x00, 0x00, 0x00 };
    static const chunk::Chunk chunks[] = {
        chunk::Chunk(false, chunk1)
    };

    static const uint8_t output[] = { 0x02, 0x11, 0x01, 0x01, 0x01, 0x00 };

    serial_test(chunk::Chunks(chunks), output);
}

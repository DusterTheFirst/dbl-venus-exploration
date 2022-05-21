#pragma once

#include <stdint.h>
#include "chunk.hpp"

namespace telemetry {
    namespace cobs {
#ifdef UNIT_TEST
        namespace test {
            extern size_t byte_offset;
            extern uint8_t *output;
        }
#endif
        void serial_write(const chunk::Chunks data);
    }
}

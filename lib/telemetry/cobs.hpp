#pragma once

#include <stdint.h>

namespace telemetry {
    namespace cobs {
        static inline void serial_write(const uint8_t *data, const size_t size);
    }
}

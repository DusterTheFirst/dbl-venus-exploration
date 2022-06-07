#pragma once

#include <Arduino.h>
#include <stdint.h>
#include <telemetry.hpp>

namespace telemetry {
    namespace command {
        typedef void (*Vector)(void);

        struct VectorTable {
            Vector CalibrateAmbientInfrared;
            Vector CalibrateReferenceInfrared;
        };

        void load_vector_table(VectorTable vector_table);
        bool process_command();
    }
}

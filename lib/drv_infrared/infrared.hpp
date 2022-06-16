#pragma once

#include <stdint.h>

/**
 * @brief Driver for interfacing with the infrared sensors.
 *
 * This is a driver subsystem. Refer to the README.md file in the lib directory
 * for more information on driver subsystems.
 */
namespace infrared {
    /**
     * @brief Initialize the infrared hardware.
     *
     * This function is called once on start up allowing for initialization
     * code to prepare the attached hardware or to put it into a known state
     */
    void init(void);

    void debug_calibration(void);

    /**
     * @brief Raw measurements from the infrared sensors
     *
     */
    namespace raw {
        uint16_t rock_left(void);
        uint16_t rock_right(void);
        uint16_t cliff_left(void);
        uint16_t cliff_right(void);
    }

    namespace detect {
        bool rock_left(void);
        bool rock_right(void);
        bool cliff_left(void);
        bool cliff_right(void);
    }

    namespace calibrate {
        void ambient(void);
        void reference(void);
    }
}

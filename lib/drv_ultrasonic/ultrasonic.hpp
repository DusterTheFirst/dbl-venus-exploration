#pragma once

#include <stdint.h>

/**
 * @brief Driver for interfacing with the ultrasonic rangefinder
 *
 * This is a driver subsystem. Refer to the README.md file in the lib directory
 * for more information on driver subsystems.
 */
namespace ultrasonic {
    /**
     * @brief Measure the distance using the ultrasonic sensor.
     * 
     * note: this method blocks for some milliseconds as it awaits the echo pulse.
     *
     * @return The calculated distance in whole centimeters.
     */
    uint16_t distance();

    /**
     * @brief Generate a pseudo-random distance for when the ultrasonic sensor is disconnected.
     * 
     * note: this method blocks for some milliseconds to mimic the wait for an echo pulse.
     *
     * @return The fake distance in whole centimeters.
     */
    uint16_t mock_distance();

    /**
     * @brief Detects if there is a mountain or not
     * 
     * @return true if there is a mountain, false if none
     */
    bool mountain();
}

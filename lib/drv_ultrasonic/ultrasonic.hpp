#pragma once

/**
 * @brief Driver for interfacing with the ultrasonic rangefinder
 *
 * This is a driver subsystem. Refer to the README.md file in the lib directory
 * for more information on driver subsystems.
 */
namespace ultrasonic {
    /**
     * @brief Retrieve the distance measured by the ultrasonic sensor
     *
     * @return The calculated distance in whole centimeters
     */
    uint64_t distance();
}

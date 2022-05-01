#pragma once

/**
 * @brief Driver for interfacing with the ultrasonic rangefinder
 * 
 * This is a driver subsystem. Refer to the README.md file in the lib directory
 * for more information on driver subsystems.
 */
namespace ultrasonic {
    /**
     * @brief Initialize the ultrasonic hardware.
     * 
     * This function is called once on start up allowing for initialization
     * code to prepare the attached hardware or to put it into a known state
     */
    void init();

    /**
     * @brief Retrieve the distance measured by the ultrasonic sensor
     * 
     * @return The calculated distance in meters
     */
    float distance();
}
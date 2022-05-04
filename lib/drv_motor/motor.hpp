#pragma once

#include <stdint.h>

/**
 * @brief Driver for interfacing with the servo motors.
 *
 * This is a driver subsystem. Refer to the README.md file in the lib directory
 * for more information on driver subsystems.
 */
namespace motor {
    /**
     * @brief Initialize the servo hardware.
     *
     * This function is called once on start up allowing for initialization
     * code to prepare the attached hardware or to put it into a known state
     */
    void init();

    enum class Direction { LEFT,
                           RIGHT };

    /**
     * @brief Actuate the grabber on the front of the robot
     *
     * @param closed The state that the grabber should be in after this actuation
     */
    void actuate_grabber(bool closed);

    /**
     * @brief Drive the robot with the specified speed
     *
     * @param distance The speed to drive in meters/second. positive = forward, negative = backwards
     */
    void drive_straight(float speed);

    struct MotorPositions {
        float left;
        float right;
    };

    /**
     * @brief Get the current motor positions
     *
     * @return Structure representing the positions of the motors
     */
    MotorPositions get_motor_positions();

    /**
     * @brief Rotate the robot in place, relative to its current heading
     *
     * @param radians The radians to rotate by, positive = left, negative = right
     */
    void rotate_robot(float radians);

    /**
     * @brief Point the ultrasonic rangefinder at the given heading
     *
     * @param heading The heading relative to the vehicle's forward vector in radians.
     * positive = left, negative = right
     */
    void point_ultrasonic(int32_t heading);
}

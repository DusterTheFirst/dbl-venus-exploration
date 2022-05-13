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

    /**
     * @brief Defines the direction in which the robot should rotate
     *
     */
    enum class Direction { LEFT,
                           RIGHT };

    /**
     * @brief Defines the state that the grabber should be in
     *
     */
    enum class GrabberPosition { OPEN,
                                 CLOSED };

    /**
     * @brief Actuate the grabber on the front of the robot
     *
     * @param closed The state that the grabber should be in after this actuation
     *
     */
    void actuate_grabber(GrabberPosition position);

    /**
     * @brief Drive the robot with the specified speed
     *
     * @param time how much should the servos move for
     * @param speed The speed to drive in meters/second. positive = forward, negative = backwards
     */
    void drive_straight(float speed, int time);

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
    void rotate_robot(float radians, Direction direction);

    /**
     * @brief Point the ultrasonic rangefinder at the given heading
     *
     * @param heading The heading relative to the vehicle's forward vector in radians.
     * positive = left, negative = right
     */

    /**
     * @brief Defines a linear interpolation for the motor speed from range of
     * [-1,1] to range of [1300,1700]
     *
     * @param motorSpeed - floating point value [-1,1]
     */
    void interpolation(float motorSpeed);
    void point_ultrasonic(int32_t heading);
}

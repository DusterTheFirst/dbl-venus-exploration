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
     * @param position The state that the grabber should be in after this actuation
     *
     */
    void actuate_grabber(GrabberPosition position);

    /**
     * @brief Drive the robot with the specified speed
     *
     * @param time how much should the servos move for
     * @param speed The speed to drive in meters/second. Value between [-1,1]
     * Negative value is forward and positive value is backward.
     * The closer the value is to 0 from both sides, the slower the motor rotates.
     * A value of 0 means that the motor stops.
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
     * @param degrees The degrees to rotate by. It is always positive.
     * @param direction The direction in which the robot shall move (left or right)
     */
    void rotate_robot(uint8_t degres, Direction direction);

    /**
     * @brief Give instructions for motors to continuously turn in a given direction
     *
     * @param direction The direction in which the robot shall move (left or right)
     */
    void turn_direction(Direction direction);

    /**
     * @brief Point the ultrasonic rangefinder at the given heading
     *
     * @param heading The heading relative to the vehicle's forward vector in radians.
     * positive = left, negative = right
     */
    void point_ultrasonic(int8_t heading);
}

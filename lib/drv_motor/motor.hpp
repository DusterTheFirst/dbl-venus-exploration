#pragma once

#include <Arduino.h>
#include <stdint.h>
#include <telemetry.hpp>

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

    // struct Movement {
    //     motor::Direction direction;
    //     uint8_t degrees = 0;
    //     float time;
    //     int speed;

    //     inline void send() {
    //         telemetry::send(F("movement:degrees"), this->degrees);
    //         telemetry::send(F("movement:direction_left"), this->direction == Direction::LEFT ? 1 : 0);
    //         // telemetry::send(F("movement:degrees"), this->degrees);
    //         // telemetry::send(F("movement:degrees"), this->degrees);
    //     }
    // };

    struct Movement {
        enum MovementType { FORWARD,
                            ROTATION };

        MovementType type;
        struct Forward {
            uint32_t time;
            int speed;
        };

        struct Rotation {
            Direction direction;
            int degrees;
        };

        union {
            struct Forward forward;
            struct Rotation rotation;
        } value;

        inline void send() {
            if (MovementType::ROTATION == type) {
                telemetry::send(F("movement:degrees"), value.rotation.degrees);
                telemetry::send(F("movement:direction_left"), value.rotation.direction == Direction::LEFT ? 1 : 0);
            } else {
                telemetry::send(F("movement:speed"), value.forward.speed);
                telemetry::send(F("movement:time"), value.forward.time);
            }
        }
    };

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
    void drive_straight(int speed, uint32_t time);

    // Speeds: 1440, 1475
    void drive_straight_a(int16_t speed, uint32_t time);

    void return_to_lab_move();
    void return_to_lab_rotate();

    // Backward
    // To Left (Right detected only)
    // To Right (Left detected only)
    int8_t rotate_to_random(int8_t where_to);

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

    /**
     * @brief Pushes an element to the movement history 'queue'
     *
     * @param movement
     */
    void pushHistory(Movement movement);

    /**
     * @brief pops the last element from the 'queue' of the movement history
     *
     * @return Movement
     */
    Movement popHistory();

    /**
     * @brief Get the last index of the 'queue'
     *
     * @return int
     */
    int getIndex();

    /**
     * @brief Get the Opposite Movement object
     *
     * @param movement
     * @return Movement
     */
    Movement getOppositeMovement(Movement movement);
}

#pragma once

/**
 * @brief Bitfields defining the different sides of the robot
 *
 * This is used to commincate between the driver modules and pathfinding.
 *
 * To use this bi
 *
 */
struct RobotSides {
    bool left : 1;
    bool right : 1;
    bool front : 1;
    bool back : 1;
};
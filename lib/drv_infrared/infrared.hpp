#pragma once

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
    void init();

    /**
     * @brief Bitfields defining the different sides of the robot
     */
    struct RobotSides {
        bool left : 1;
        bool right : 1;
        bool front : 1;
        bool back : 1;
    };

    /**
     * @brief Detect presence of a cliff or boundary
     * 
     * @return A structure describing the edge status of the four sides of the robot
     */
    RobotSides sees_edge();

    /**
     * @brief Detect presence of a rock
     * 
     * @return A structure describing the rock status of the four sides of the robot
     */
    RobotSides sees_rock();

}

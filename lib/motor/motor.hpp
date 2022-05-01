#pragma once

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
     * @brief Actuate the grabber on the front of the robot
     * 
     * @param closed The state that the grabber should be in after this actuation
     */
    void actuate_grabber(bool closed);

    /**
     * @brief Drive the robot forward the specified distance
     * 
     * @param distance The distance to drive in meters
     */
    void drive_forward(double distance);

    /**
     * @brief Rotate the robot in place, relative to its current heading
     * 
     * @param radians The radians to rotate by, positive = left, negative = right
     */
    void rotate_robot(double radians);
    
    /**
     * @brief Point the ultrasonic rangefinder at the given heading
     * 
     * @param heading The heading relative to the vehicle's forward vector in radians.
     * positive = left, negative = right
     */
    void point_ultrasonic(double heading);
}
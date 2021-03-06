#pragma once
#include <I2Cdev.h>
#include <MPU6050_6Axis_MotionApps20.h>

/**
 * @brief Driver for interfacing with the gyroscope
 *
 * This is a driver subsystem. Refer to the README.md file in the lib directory
 * for more information on driver subsystems.
 */
namespace gyro {

    /**
     * @brief initialise the gyroscope with correct parameters
     *
     */
    void init();

    /**
     * @brief initialise the MPU with correct parameters
     *
     */
    void initMPU();

    /**
     * @brief Retrieve the distance measured by the ultrasonic sensor
     *
     * @return [-180,180] degrees of rotation. Positive angle means
     * rotation to the right. Negative angle means rotation to the left.
     */
    int16_t get_angle();

    /**
     * @brief Get the angle of the gyro modulo 360
     *
     * @return uint16_t - angle between [0,360] instead of [-180,180]
     */
    uint16_t get_angle_modulo(int32_t angle);

    /**
     * @brief Get the acceleration
     *
     * @return Positive acceleration means forward movement.
     * Negative acceleration means backward movement.
     */
    float get_acceleration();

    /**
     * @brief Get the acute between two positive angles
     *
     * @return The acute angle in [0, 180] between the two angles
     */
    uint8_t get_acute_angle(uint16_t start, uint16_t end);
}

#include <Arduino.h>
#include <motor.hpp>

void motor::init() {
    // TODO: Write initialization code
}

void motor::actuate_grabber(bool closed) {
    // TODO: move the grabber arm
}

void motor::drive_straight(float speed) {
    // TODO: drive both motors with the given speed, ensuring that
    // the motors turn at the same speed as to not rotate
}

void motor::rotate_robot(float radians) {
    // TODO: drive each motor in opposite directions, causing the robot to rotate
    // the specified amount
}

void motor::point_ultrasonic(float heading) {
    // TODO: command the ultrasonic's servo to point at the given heading
}

motor::MotorPositions motor::get_motor_positions() {
    // TODO: using an interrupt or somthing, keep track of the motor position
    // reported by the rotary encoders

    return {
        .left = 0.0, // TODO: Replace these constants with the actual calculated
                     // values. These exist just to show how you would construct
                     // the MotorPositions structure
        .right = 0.0
    };
}
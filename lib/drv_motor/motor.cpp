#include <Arduino.h>
#include <motor.hpp>

void motor::init() {
    // TODO: Write initialization code
}

void motor::actuate_grabber(bool closed) {
    // TODO: move the grabber arm
}

void motor::drive_straight(double speed) {
    // TODO: drive both motors with the given speed, ensuring that
    // the motors turn at the same speed as to not rotate
}

void motor::rotate_robot(double radians) {
    // TODO: drive each motor in opposite directions, causing the robot to rotate
    // the specified amount
}

void motor::point_ultrasonic(double heading) {
    // TODO: command the ultrasonic's servo to point at the given heading
}
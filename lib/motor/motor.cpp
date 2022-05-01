#include <Arduino.h>
#include <motor.hpp>

void motor::init() {
    // TODO: Write initialization code
}

void motor::actuate_grabber(bool closed) {
    // TODO: move the grabber arm
}

// FIXME:(zach) Figure out a way to check for obsticles while this is happening?
void motor::drive_forward(double distance) {
    // TODO: drive both motors forward the given distance, ensuring that
    // the motors turn at the same speed
}

void motor::rotate_robot(double radians) {
    // TODO: drive each motor in opposite directions, causing the robot to rotate
    // the specified amount
}

void motor::point_ultrasonic(double heading) {
    // TODO: command the ultrasonic's servo to point at the given heading
}
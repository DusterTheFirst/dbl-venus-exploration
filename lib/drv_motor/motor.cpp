#include <Arduino.h>
#include <Servo.h>
#include <motor.hpp>

#define SERVO_LEFT_PIN 12
#define SERVO_RIGHT_PIN 13
#define GRABBER_PIN 10

const int motorPulseLow = 1300;  // at this point the motor is a full speed clockwise
const int motorPulseHigh = 1700; // at this point the motor is a full speed anticlockwise
const int linearValueLow = -1;   // left margin of the interpolation values
const int linearValueHigh = 1;   // right margin of the interpolation values

Servo servoLeft;
Servo servoRight;
Servo grabber;
bool grabberClosed; // true if grabber is closed, false otherwise

void motor::init() {
    // TODO: Write initialization code
    servoLeft.attach(SERVO_LEFT_PIN);
    servoRight.attach(SERVO_RIGHT_PIN);
    grabber.attach(GRABBER_PIN);
}

void motor::actuate_grabber(GrabberPosition position) {
    // TODO: move the grabber arm

    if (!grabberClosed && position == GrabberPosition::OPEN) { // should close the grabber
        grabber.write(0);
        grabberClosed = !grabberClosed;
    } else if (grabberClosed && position == GrabberPosition::CLOSED) { // should open the grabber
        grabber.write(180);
        grabberClosed = !grabberClosed;
    }
}

void motor::drive_straight(float speed, int time) {
    // TODO: drive both motors with the given speed, ensuring that
    // the motors turn at the same speed as to not rotate
    servoLeft.writeMicroseconds(time);
    servoRight.writeMicroseconds(time);
}

void motor::rotate_robot(float radians, Direction direction) {
    // TODO: drive each motor in opposite directions, causing the robot to rotate
    // the specified amount

    if (direction == motor::Direction::LEFT) { // has to move left

    } else if (direction == motor::Direction::RIGHT) { // has to move right

    } else { // throw error
    }
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

int interpolation(float motorSpeed) {

    return motorPulseLow + (motorSpeed - linearValueLow) * (motorPulseHigh - motorPulseLow) / (linearValueHigh - linearValueLow);
}
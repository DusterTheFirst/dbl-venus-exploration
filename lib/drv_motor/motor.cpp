#include <Arduino.h>
#include <Servo.h>
#include <motor.hpp>
#include <telemetry.hpp>

#define SERVO_LEFT_PIN 12
#define SERVO_RIGHT_PIN 13
#define GRABBER_PIN 10

const int motorPulseLow = 1300;  // at this point the motor is a full speed clockwise
const int motorPulseHigh = 1700; // at this point the motor is a full speed anticlockwise
const int linearValueLow = -1;   // left margin of the interpolation values
const int linearValueHigh = 1;   // right margin of the interpolation values
const int grabberDegreesClosed = 0;
const int grabberDegreesOpen = 180;

Servo servoLeft;
Servo servoRight;
Servo grabber;
bool grabberClosed; // true if grabber is closed, false otherwise

void motor::init() {
    gripper_servo.attach(GRIPPER_SERVO);
    ultrasonic_servo.attach(ULTRASONIC_SERVO);

    left_drive.attach(LEFT_DRIVE);
    right_drive.attach(RIGHT_DRIVE);

    // TODO: Write initialization code
    servoLeft.attach(SERVO_LEFT_PIN);
    servoRight.attach(SERVO_RIGHT_PIN);
    grabber.attach(GRABBER_PIN);
}

void motor::actuate_grabber(GrabberPosition position) {
    // TODO: move the grabber arm

    if (!grabberClosed && position == GrabberPosition::OPEN) { // should close the grabber
        grabber.write(grabberDegreesClosed);
        grabberClosed = !grabberClosed;
    } else if (grabberClosed && position == GrabberPosition::CLOSED) { // should open the grabber
        grabber.write(grabberDegreesOpen);
        grabberClosed = !grabberClosed;
    }
}

void motor::drive_straight(float speed, int time) {
    // TODO: drive both motors with the given speed, ensuring that
    // the motors turn at the same speed as to not rotate

    telemetry::send("motor:drive_speed", speed);

    int32_t delta = speed * 200;
    int32_t microseconds = 1500 + delta;

    telemetry::send("motor:drive_micros", microseconds);

    left_drive.writeMicroseconds(microseconds);
    right_drive.writeMicroseconds(microseconds);
}

void motor::rotate_robot(float radians, Direction direction) {
    // TODO: drive each motor in opposite directions, causing the robot to rotate
    // the specified amount

    if (direction == motor::Direction::LEFT) { // has to move left

    } else if (direction == motor::Direction::RIGHT) { // has to move right

    } else { // throw error
    }
}

void motor::point_ultrasonic(int32_t heading) {
    // Clamp into known range
    if (heading < -90) {
        heading = -90;
    }

    if (heading > 90) {
        heading = 90;
    }

    // Change reference to left facing = 0
    ultrasonic_servo.write(heading + 90);
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
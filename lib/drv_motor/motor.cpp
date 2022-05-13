#include <Arduino.h>
#include <Servo.h>
#include <motor.hpp>
#include <telemetry.hpp>

#define SERVO_LEFT_PIN 12
#define SERVO_RIGHT_PIN 13
#define GRABBER_PIN 10
#define ULTRASONIC_SERVO_PIN 9

#define GRABBER_DEGREES_CLOSED 0
#define GRABBER_DEGREES_OPEN 180

Servo servoLeft, servoRight, grabberServo, ultrasonicServo;

void motor::init() {
    grabberServo.attach(GRABBER_PIN);
    ultrasonicServo.attach(ULTRASONIC_SERVO_PIN);
    servoLeft.attach(SERVO_LEFT_PIN);
    servoRight.attach(SERVO_RIGHT_PIN);
}

void motor::actuate_grabber(GrabberPosition position) {
    switch (position) {
        case GrabberPosition::OPEN: {
            // close the grabber
            grabberServo.write(GRABBER_DEGREES_CLOSED);

            break;
        }
        case GrabberPosition::CLOSED: {
            // open the grabber
            grabberServo.write(GRABBER_DEGREES_OPEN);

            break;
        }
    }
}

/**
 * @brief Defines a linear interpolation for the motor speed from range of
 * [-1,1] to range of [1300,1700]
 *
 * @param motorSpeed - floating point value [-1,1]
 */
int16_t interpolation(float motorSpeed) {
    // at this point the motor is a full speed clockwise
    const int16_t motorPulseLow = 1300;
    // at this point the motor is a full speed anticlockwise
    const int16_t motorPulseHigh = 1700;
    // left margin of the interpolation values
    const int8_t linearValueLow = -1;
    // right margin of the interpolation values
    const int8_t linearValueHigh = 1;

    return motorPulseLow +
           (motorSpeed - linearValueLow) *
               (motorPulseHigh - motorPulseLow) /
               (linearValueHigh - linearValueLow);
}

void motor::drive_straight(float speed, int time) {
    // TODO: drive both motors with the given speed, ensuring that
    // the motors turn at the same speed as to not rotate
    // telemetry::send("motor:drive_speed", speed);
    // telemetry::send("motor:drive_micros", microseconds);
    int interpolatedSpeed = interpolation(speed);

    servoLeft.writeMicroseconds(interpolatedSpeed);
    servoRight.writeMicroseconds(interpolatedSpeed);
}

void motor::rotate_robot(float radians, Direction direction) {
    // TODO: drive each motor in opposite directions, causing the robot to
    // rotate the specified amount

    if (direction == motor::Direction::LEFT) { // has to move left
        // TODO:
    } else if (direction == motor::Direction::RIGHT) { // has to move right
        // TODO:
    }
}

void motor::point_ultrasonic(int8_t heading) {
    // Clamp into known range
    if (heading < -90) {
        heading = -90;
    }

    if (heading > 90) {
        heading = 90;
    }

    // Change reference to left facing = 0
    ultrasonicServo.write(heading + 90);
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

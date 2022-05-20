#include "motor.hpp"
#include "gyro.hpp"
#include <Arduino.h>
#include <Servo.h>
#include <telemetry.hpp>

#define SERVO_LEFT_PIN 12
#define SERVO_RIGHT_PIN 13
#define GRABBER_PIN 10
#define ULTRASONIC_SERVO_PIN 11

#define GRABBER_DEGREES_CLOSED 0
#define GRABBER_DEGREES_OPEN 120

#define MOTOR_STOP 1500

Servo servoLeft, servoRight, grabberServo, ultrasonicServo;

void motor::init() {
    grabberServo.attach(GRABBER_PIN);
    grabberServo.write(GRABBER_DEGREES_OPEN);

    ultrasonicServo.attach(ULTRASONIC_SERVO_PIN);
    ultrasonicServo.write(0);

    servoLeft.attach(SERVO_LEFT_PIN);
    servoLeft.write(MOTOR_STOP);

    servoRight.attach(SERVO_RIGHT_PIN);
    servoRight.write(MOTOR_STOP);
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
    telemetry::send("motor:grabber_open",
                    position == GrabberPosition::OPEN ? true : false);
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

    telemetry::send("motor:drive_speed", speed);
    int interpolatedSpeed = interpolation(speed);

    telemetry::send("motor:drive_micros", interpolatedSpeed);
    servoLeft.writeMicroseconds(interpolatedSpeed);
    servoRight.writeMicroseconds(interpolatedSpeed);
}

void motor::rotate_robot(uint8_t degrees, Direction direction) {
    // TODO: drive each motor in opposite directions, causing the robot to
    // rotate the specified amount
    uint16_t destinationAngle = gyro::get_angle();
    uint16_t currentAngle;

    switch (direction) {
        case Direction::RIGHT: {
            destinationAngle += degrees;
            break;
        }
        case Direction::LEFT: {
            destinationAngle -= degrees;
            break;
        }
    }

    do {
        currentAngle = gyro::get_angle();
        switch (direction) {
            case Direction::RIGHT: {
                // move right
                break;
            }
            case Direction::LEFT: {
                // move left
                break;
            }
        }
    } while (
        (direction == Direction::RIGHT && currentAngle < destinationAngle) ||
        (direction == Direction::LEFT && currentAngle > destinationAngle));

    telemetry::send("motor:rotation_angle", degrees);
}

/**
 * @brief clamps heading between [-90,90]
 *
 * @param heading The heading relative to the vehicle's forward vector in degrees.
 * positive = left, negative = right
 */
inline int8_t clamp_heading(int8_t heading) {
    if (heading < -90) {
        return -90;
    } else if (heading > 90) {
        return 90;
    } else {
        return heading;
    }
}

void motor::point_ultrasonic(int8_t heading) {
    static int8_t pre_heading = 0;

    heading = clamp_heading(heading);
    telemetry::send("ultrasonic:heading", heading);

    // Calculate the difference in the previous heading and the current heading
    uint8_t diff;
    if (heading > pre_heading) {
        diff = heading - pre_heading;
    } else {
        diff = pre_heading - heading;
    }
    telemetry::send("ultrasonic:heading2", heading);

    // Change reference to left facing = 0
    uint8_t servo_command = 180 - (heading + 90);

    // Extra debug telemetry, normally unnecessary
    telemetry::send("ultrasonic:heading.servo_command", servo_command);
    telemetry::send("ultrasonic:heading.pre_heading", pre_heading);
    telemetry::send("ultrasonic:heading.diff", diff);

    pre_heading = heading;

    ultrasonicServo.write(servo_command);

    delay(diff * 5);
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

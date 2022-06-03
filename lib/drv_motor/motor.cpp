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
#define MOTOR_ROTATE_SPEED 30

Servo servoLeft, servoRight, grabberServo, ultrasonicServo;

void motor::init() {
    grabberServo.attach(GRABBER_PIN);
    grabberServo.write(GRABBER_DEGREES_CLOSED);

    ultrasonicServo.attach(ULTRASONIC_SERVO_PIN);
    ultrasonicServo.write(0);

    servoLeft.attach(SERVO_LEFT_PIN);
    servoLeft.write(MOTOR_STOP);

    servoRight.attach(SERVO_RIGHT_PIN);
    servoRight.write(MOTOR_STOP);
}

void stop_motor() {
    servoLeft.write(MOTOR_STOP);
    servoLeft.detach();

    servoRight.write(MOTOR_STOP);
    servoRight.detach();
}

void start_motor() {
    servoLeft.write(MOTOR_STOP);
    servoLeft.attach(SERVO_LEFT_PIN);

    servoRight.write(MOTOR_STOP);
    servoRight.attach(SERVO_RIGHT_PIN);
}

void motor::turn_direction(Direction direction) {
    switch (direction) {
        case Direction::RIGHT: {
            servoLeft.writeMicroseconds(MOTOR_STOP + MOTOR_ROTATE_SPEED);
            servoRight.writeMicroseconds(MOTOR_STOP + MOTOR_ROTATE_SPEED);
            break;
        }
        case Direction::LEFT: {
            servoLeft.writeMicroseconds(MOTOR_STOP - MOTOR_ROTATE_SPEED);
            servoRight.writeMicroseconds(MOTOR_STOP - MOTOR_ROTATE_SPEED);
            break;
        }
    }
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
    telemetry::send(F("motor:grabber_open"),
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

    telemetry::send(F("motor:drive_speed"), speed);
    int interpolatedSpeed = interpolation(speed);

    telemetry::send(F("motor:drive_micros"), interpolatedSpeed);
    servoLeft.writeMicroseconds(interpolatedSpeed);
    servoRight.writeMicroseconds(interpolatedSpeed);
}

void motor::rotate_robot(uint8_t degrees, Direction direction) {

    start_motor();
    // TODO: drive each motor in opposite directions, causing the robot to
    // rotate the specified amount

    // motor::stop_motor();

    int32_t destinationAngleModulo = gyro::get_angle_modulo(gyro::get_angle());
    int32_t initialAngleModulo = gyro::get_angle_modulo(gyro::get_angle());

    telemetry::send(F("motor:initial_angle"), destinationAngleModulo);
    telemetry::send(F("motor:turning_right"), direction == Direction::RIGHT);
    uint16_t currentAngleModulo;
    bool crossedBoundary = false;

    switch (direction) {
        case Direction::RIGHT: {
            destinationAngleModulo += degrees;
            // crossedBoundary = destinationAngleModulo > 360 ? true : false;
            break;
        }
        case Direction::LEFT: {
            destinationAngleModulo -= degrees;
            // crossedBoundary = destinationAngleModulo < 0 ? true : false;
            break;
        }
    }

    destinationAngleModulo = gyro::get_angle_modulo(destinationAngleModulo);

    int previous_angle = gyro::get_acute_angle(destinationAngleModulo, initialAngleModulo);
    int current_angle;
    telemetry::send(F("motor:destination_angle"), destinationAngleModulo);
    telemetry::send(F("motor:acute_angle"), previous_angle);

    do {
        currentAngleModulo = gyro::get_angle_modulo(gyro::get_angle());
        current_angle = gyro::get_acute_angle(currentAngleModulo, destinationAngleModulo);
        telemetry::send(F("motor:current_angle"), currentAngleModulo);
        // detect when boundary is crossed

        // if (Direction::LEFT == direction && currentAngleModulo > initialAngleModulo) {
        //     crossedBoundary = true;
        // }

        // if (Direction::RIGHT == direction && currentAngleModulo < initialAngleModulo) {
        //     crossedBoundary = true;
        // }

        switch (direction) {
            case Direction::RIGHT: {
                motor::turn_direction(Direction::RIGHT);

                break;
            }
            case Direction::LEFT: {
                motor::turn_direction(Direction::LEFT);
                break;
            }
        }
        if (motor::rotation_destination_reached(previous_angle, current_angle))
            break;
        previous_angle = current_angle;
    } while (true);

    telemetry::send(F("motor:rotation_angle"), degrees);
    stop_motor();
}

bool motor::rotation_destination_reached(int previous_angle, int current_angle) {
    return previous_angle < current_angle || current_angle <= 3;
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
    telemetry::send(F("ultrasonic:heading"), heading);

    // Calculate the difference in the previous heading and the current heading
    uint8_t diff;
    if (heading > pre_heading) {
        diff = heading - pre_heading;
    } else {
        diff = pre_heading - heading;
    }

    // Change reference to left facing = 0
    uint8_t servo_command = 180 - (heading + 90);

    // Extra debug telemetry, normally unnecessary
    telemetry::send(F("ultrasonic:heading.servo_command"), servo_command);
    telemetry::send(F("ultrasonic:heading.pre_heading"), pre_heading);
    telemetry::send(F("ultrasonic:heading.diff"), diff);

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

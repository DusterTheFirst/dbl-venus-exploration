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

motor::Movement movementHistory[20];
int index = 0;

void motor::pushHistory(Movement movement) {
    movementHistory[index++] = movement;
}

motor::Movement motor::popHistory() {
    return movementHistory[--index];
}

motor::Movement motor::getOppositeMovement(Movement movement) {
    Movement oppositeMovement;
    if (movement.degrees != 0) {
        oppositeMovement.direction = movement.direction == Direction::LEFT ? Direction::RIGHT : Direction::LEFT;
        oppositeMovement.degrees = movement.degrees;
        oppositeMovement.speed = 0;
        oppositeMovement.time = 0;
    } else {
        oppositeMovement.speed = 3000 - movement.speed;
        oppositeMovement.time = movement.time;
        oppositeMovement.degrees = 0;
        oppositeMovement.direction = Direction::LEFT;
    }

    return oppositeMovement;
}

int motor::getIndex() {
    return index;
}

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

    start_motor();
    telemetry::send(F("motor:drive_speed"), speed);
    int interpolated_speed = interpolation(speed);

    long start = millis();
    while (millis() - start <= time) {

        // telemetry::send(F("motor:drive_micros"), interpolated_speed);
        // servoLeft.writeMicroseconds(interpolated_speed);
        // servoRight.writeMicroseconds(3000 - interpolated_speed);
        telemetry::send(F("motor:drive_micros"), speed);
        servoLeft.writeMicroseconds(3000 - speed);
        servoRight.writeMicroseconds(speed);
    }

    stop_motor();
}

bool rotation_destination_reached(int previous_angle, int current_angle) {
    return previous_angle < current_angle || current_angle <= 3;
}

void motor::rotate_robot(uint8_t degrees, Direction direction) {
    uint16_t initial_angle_mod = gyro::get_angle_modulo(gyro::get_angle());

    telemetry::send(F("motor:initial_angle"), initial_angle_mod);
    telemetry::send(F("motor:turning_right"), direction == Direction::RIGHT);

    // Calculate the destination angle
    int32_t destination_angle;
    switch (direction) {
        case Direction::RIGHT: {
            destination_angle = initial_angle_mod + degrees;
            break;
        }
        case Direction::LEFT: {
            destination_angle = initial_angle_mod - degrees;
            break;
        }
    }

    // Fit the angle to [0, 360)
    uint16_t destination_angle_mod = gyro::get_angle_modulo(destination_angle);

    int previous_acute = gyro::get_acute_angle(destination_angle_mod,
                                               initial_angle_mod);
    telemetry::send(F("motor:destination_angle"), destination_angle_mod);
    telemetry::send(F("motor:acute_angle"), previous_acute);

    uint16_t current_angle_mod;
    uint8_t current_acute;

    start_motor();
    while (true) {
        current_angle_mod = gyro::get_angle_modulo(gyro::get_angle());

        current_acute = gyro::get_acute_angle(current_angle_mod,
                                              destination_angle_mod);
        telemetry::send(F("motor:current_angle"), current_angle_mod);

        motor::turn_direction(direction);

        if (rotation_destination_reached(previous_acute, current_acute)) {
            break;
        }

        previous_acute = current_acute;
    }
    stop_motor();

    telemetry::send(F("motor:rotation_angle"), degrees);
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

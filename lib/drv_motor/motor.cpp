#include "motor.hpp"
#include "gyro.hpp"

#include "infrared.hpp"
#include "ultrasonic.hpp"

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

Servo servo_left, servo_right, grabber_servo, ultrasonic_servo;

// 1475 and 1440 respectively
const float SERVO_VEL[2] = { 0.0415, 0.1172 };

motor::Movement movement_history[20];
int index = 0;

void motor::push_history(Movement movement) {
    movement_history[index++] = movement;
}

motor::Movement motor::pop_history() {
    return movement_history[--index];
}

motor::Movement motor::get_opposite_movement(Movement movement) {
    Movement opposite_movement;
    if (movement.type == movement.ROTATION) {
        opposite_movement.value.rotation.direction = movement.value.rotation.direction == Direction::LEFT ? Direction::RIGHT : Direction::LEFT;
        opposite_movement.value.rotation.degrees = movement.value.rotation.degrees;
    } else if (movement.type == movement.FORWARD) {
        opposite_movement.value.forward.speed = 3000 - movement.value.forward.speed;
        opposite_movement.value.forward.time = movement.value.forward.time;
    }
    opposite_movement.type = movement.type;

    return opposite_movement;
}

int motor::get_index() {
    return index;
}

void motor::init() {
    grabber_servo.attach(GRABBER_PIN);
    grabber_servo.write(GRABBER_DEGREES_CLOSED);

    ultrasonic_servo.attach(ULTRASONIC_SERVO_PIN);
    ultrasonic_servo.write(0);

    servo_left.attach(SERVO_LEFT_PIN);
    servo_left.write(MOTOR_STOP);

    servo_right.attach(SERVO_RIGHT_PIN);
    servo_right.write(MOTOR_STOP);
}

void motor::stop_motor() {
    servo_left.write(MOTOR_STOP);
    servo_left.detach();

    servo_right.write(MOTOR_STOP);
    servo_right.detach();
}

void motor::start_motor() {
    servo_left.write(MOTOR_STOP);
    servo_left.attach(SERVO_LEFT_PIN);

    servo_right.write(MOTOR_STOP);
    servo_right.attach(SERVO_RIGHT_PIN);
}

void motor::turn_direction(Direction direction) {
    switch (direction) {
        case Direction::RIGHT: {
            servo_left.writeMicroseconds(MOTOR_STOP + MOTOR_ROTATE_SPEED);
            servo_right.writeMicroseconds(MOTOR_STOP + MOTOR_ROTATE_SPEED);
            break;
        }
        case Direction::LEFT: {
            servo_left.writeMicroseconds(MOTOR_STOP - MOTOR_ROTATE_SPEED);
            servo_right.writeMicroseconds(MOTOR_STOP - MOTOR_ROTATE_SPEED);
            break;
        }
    }
}

void motor::actuate_grabber(GrabberPosition position) {
    switch (position) {
        case GrabberPosition::OPEN: {
            // close the grabber
            grabber_servo.write(GRABBER_DEGREES_CLOSED);

            break;
        }
        case GrabberPosition::CLOSED: {
            // open the grabber
            grabber_servo.write(GRABBER_DEGREES_OPEN);

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

void motor::drive_straight(int speed, uint32_t time) {
    start_motor();

    long start = millis();
    while (millis() - start <= time) {

        servo_left.writeMicroseconds(3000 - speed);
        servo_right.writeMicroseconds(speed);
    }

    stop_motor();
}

void motor::drive_straight(int speed) {
    start_motor();
    
    servo_left.writeMicroseconds(3000 - speed);
    servo_right.writeMicroseconds(speed);

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
        // telemetry::send(F("motor:current_angle"), current_angle_mod);

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

#if MOTOR_ULTRASONIC_DEBUG
    // Extra debug telemetry, normally unnecessary
    telemetry::send(F("ultrasonic:heading:servo_command"), servo_command);
    telemetry::send(F("ultrasonic:heading:pre_heading"), pre_heading);
    telemetry::send(F("ultrasonic:heading:diff"), diff);
#endif

    pre_heading = heading;

    ultrasonic_servo.write(servo_command);

    delay(diff * 5);
}
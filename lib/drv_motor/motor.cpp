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

// 1475 and 1440 respectively
const float SERVO_VEL[2] = {0.0415, 0.1172};

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
    if (movement.type == movement.ROTATION) {
        oppositeMovement.value.rotation.direction = movement.value.rotation.direction == Direction::LEFT ? Direction::RIGHT : Direction::LEFT;
        oppositeMovement.value.rotation.degrees = movement.value.rotation.degrees;
    } else if (movement.type == movement.FORWARD) {
        oppositeMovement.value.forward.speed = 3000 - movement.value.forward.speed;
        oppositeMovement.value.forward.time = movement.value.forward.time;
    }
    oppositeMovement.type = movement.type;

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

void motor::drive_straight(int speed, uint32_t time) {

    start_motor();
    telemetry::send(F("motor:drive_speed"), speed);
    // int interpolated_speed = interpolation(speed);

    long start = millis();
    while (millis() - start <= time) {

        // telemetry::send(F("motor:drive_micros"), interpolated_speed);
        // servoLeft.writeMicroseconds(interpolated_speed);
        // servoRight.writeMicroseconds(3000 - interpolated_speed);
        // telemetry::send(F("motor:drive_micros"), speed);
        servoLeft.writeMicroseconds(3000 - speed);
        servoRight.writeMicroseconds(speed);
    }

    stop_motor();
}

// Send -1 for no time
void motor::drive_straight_a(int16_t speed, uint32_t time) {
    Movement ret;
    ret.type = FORWARD;
    ret.value.forward.speed = speed;

    servoLeft.attach(SERVO_LEFT_PIN);
    servoRight.attach(SERVO_RIGHT_PIN);

        /* 0 - Default
     * 1 - Rock
     * 2 - Left cliff
     * 3 - Right cliff
     * 4 - Forward cliff
     * 5 - Ultrasound
     */
    uint8_t end_condition = 0;
    uint32_t start_time = millis()
    uint32_t end_time;

    while (end_time <= time) {
        end_time = millis() - start_time();
        if (infared::test_detect_rock() {
            end_condition = 1;
            break;
        }
        else if (infared::test_detect_cliff() ) {

        }
        else if (ultrasonic::distance() < 10) {
            end_condition = 5;
            break;
        } 
    }
    ret.value.forward.time = end_time;
    pushHistory(ret);

    switch(end_condition) {
        case 1:
            actuate_grabber(GrabberPosition::OPEN);
            // Align to rock
            actuate_grabber(GrabberPosition::CLOSED); 
            return_to_lab_move();
            break;
        case 2:
        default:
            break;
    }
}

void return_to_lab_move() {
    Movement last_rev = getOppositeMovement(popHistory());
    if (index == 0) return;
    else {
        drive_straight_a(last_rev.value.forward.speed, last_rev.value.forward.time);
        return_to_lab_rotate();
    }
}

void return_to_lab_rotate() {
    Movement last_rev = getOppositeRotation(popHistory());
    if (index == 0) return;
    else {
        rotate_robot (last_rev.value.rotation.degrees, last_rev.value.rotation.direction);
        return_to_lab_move();
    }
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

    // Extra debug telemetry, normally unnecessary
    telemetry::send(F("ultrasonic:heading.servo_command"), servo_command);
    telemetry::send(F("ultrasonic:heading.pre_heading"), pre_heading);
    telemetry::send(F("ultrasonic:heading.diff"), diff);

    pre_heading = heading;

    ultrasonicServo.write(servo_command);

    delay(diff * 5);
}
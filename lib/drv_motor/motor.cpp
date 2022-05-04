#include <Arduino.h>
#include <Servo.h>
#include <motor.hpp>
#include <telemetry.hpp>

#define SERVO_LEFT_PIN 12
#define SERVO_RIGHT_PIN 13

Servo servoLeft;
Servo servoRight;
bool grabberClosed; // true if grabber is closed, false otherwise

void motor::init() {
    gripper_servo.attach(GRIPPER_SERVO);
    ultrasonic_servo.attach(ULTRASONIC_SERVO);

    left_drive.attach(LEFT_DRIVE);
    right_drive.attach(RIGHT_DRIVE);

    // TODO: Write initialization code
    servoLeft.attach(SERVO_LEFT_PIN);
    servoRight.attach(SERVO_RIGHT_PIN);
}

void motor::actuate_grabber(GrabberPosition position) {
    // TODO: move the grabber arm

    if (!grabberClosed && position == GrabberPosition::OPEN) { // should close the grabber

    } else if (grabberClosed && position == GrabberPosition::CLOSED) { // should open the grabber

    } else { // do nothing...
    }
}

void motor::drive_straight(float speed) {
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

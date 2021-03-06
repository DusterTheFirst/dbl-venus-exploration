#include <Arduino.h>

// ----- Include telemetry -----
#include <command.hpp>
#include <telemetry.hpp>

// ----- Include drivers -----
#include <gyro.hpp>
#include <infrared.hpp>
#include <motor.hpp>
#include <ultrasonic.hpp>

// ----- Include pathfinding -----
#include <pathfinding.hpp>

// ----- Include timekeeping -----
#include <time.hpp>

bool turnedOnce;

void setup() {
    telemetry::init();
    randomSeed(analogRead(0));

    // Give the serial port time to connect
    delay(1000);

    telemetry::command::load_vector_table({
        .CalibrateAmbientInfrared = infrared::calibrate::ambient,
        .CalibrateReferenceInfrared = infrared::calibrate::reference,
    });

    telemetry::send(F("main:initializing"), true);
    telemetry::send(F("main:running"), false);

    gyro::init();
    // delay(3000);
    motor::init();
    infrared::init();

    telemetry::send(F("main:initializing"), false);
    telemetry::send(F("main:running"), true);

    // motor::rotate_robot(50, motor::Direction::RIGHT);
    // delay(5000);
    // motor::rotate_robot(140, motor::Direction::LEFT);
    motor::point_ultrasonic(0);

    turnedOnce = false;

    time::init();
}

void loop() {
    // telemetry::send(F("history"), true);
    //   motor::rotate_robot(50, motor::Direction::RIGHT);

    /*
    if (time::is_before(5000) && !turnedOnce) {
        motor::drive_straight(1450, 2000);
        motor::Movement movement;
        motor::Movement::Forward forward;
        forward.speed = 1450;
        forward.time = 2000;
        movement.type = movement.FORWARD;
        movement.value.forward = forward;
        motor::push_history(movement);

        motor::Movement movement2;
        motor::rotate_robot(50, motor::Direction::RIGHT);
        motor::Movement::Rotation rotation;
        movement2.type = movement2.ROTATION;
        rotation.direction = motor::Direction::RIGHT;
        rotation.degrees = 50;
        movement2.value.rotation = rotation;
        motor::push_history(movement2);
        movement2.send();
        turnedOnce = true;
    } else if (time::is_after(5000) && time::is_before(8000) && turnedOnce) {
        // telemetry::send(F("motor:rotation_angle"), 69);
        motor::rotate_robot(140, motor::Direction::LEFT);
        motor::Movement movement;
        motor::Movement::Rotation rotation;
        movement.type = movement.ROTATION;
        rotation.direction = motor::Direction::LEFT;
        rotation.degrees = 140;
        movement.value.rotation = rotation;
        motor::push_history(movement);
        movement.send();
        turnedOnce = false;
    }

    if (time::is_after(10000)) {
        // telemetry::send(F("history"), true);
        if (motor::get_index() >= 1) {
            motor::Movement movement = motor::get_opposite_movement(motor::pop_history());
            if (movement.type == movement.ROTATION) {
                telemetry::send(F("motor:degrees_rotation_after"), movement.value.rotation.degrees);
                motor::rotate_robot(movement.value.rotation.degrees, movement.value.rotation.direction);
            } else if (movement.type == movement.FORWARD) {
                motor::drive_straight(movement.value.forward.speed, movement.value.forward.time);
            }
        }
    }
    */
    // motor::point_ultrasonic(heading);

    // // uint16_t distance = ultrasonic::distance();
    // uint16_t distance = ultrasonic::mock_distance();

    // telemetry::send(F("ultrasonic:distance"), distance);
    // last_readings[heading + 90] = distance;

    // if (heading % 5 == 0) {
    //     telemetry::send_arr(F("ultrasonic:last_readings"), last_readings,
    //                    sizeof(last_readings) / sizeof(last_readings[0]));
    // }

    // if (heading >= 90) {
    //     step = -STEP_BY;
    // } else if (heading <= -90) {
    //     step = STEP_BY;
    // }

    // heading += step;
    pathfinding::random_strategy(1475);
    // delay(2000);
    // motor::rotate_robot(10, motor::Direction::LEFT);
    // delay(10000);
}

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
#include <time.h>

bool turnedOnce;

void setup() {
    telemetry::init();

    // Give the serial port time to connect
    delay(1000);

    telemetry::command::load_vector_table({
        .CalibrateAmbientInfrared = infrared::calibrate_ambient,
        .CalibrateReferenceInfrared = infrared::calibrate_reference,
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

    turnedOnce = false;

    time::init();
}

#define STEP_BY 1

int8_t heading = -90;
int8_t step = STEP_BY;

uint16_t last_readings[181] = { 0 };

void loop() {
    // Call into command service routines if a command has been received
    telemetry::command::process_command();

    uint16_t test = infrared::test_raw();
    float voltage = ((float)test / (float)(1 << 10)) * 5.0;
    telemetry::send(F("infrared:test"), test);
    telemetry::send(F("infrared:voltage"), voltage);

    telemetry::send(F("infrared:rock"), infrared::test_detect_rock());
    telemetry::send(F("infrared:cliff"), infrared::test_detect_cliff());

    delay(100);
    
    // telemetry::send(F("history"), true);
    //   motor::rotate_robot(50, motor::Direction::RIGHT);

    // if (time::is_before(5000) && !turnedOnce) {
    //     motor::drive_straight(1450, 2000);
    //     motor::Movement movement;
    //     movement.speed = 1450;
    //     movement.time = 2000;
    //     motor::pushHistory(movement);

    //     motor::rotate_robot(50, motor::Direction::RIGHT);
    //     movement.direction = motor::Direction::RIGHT;
    //     movement.degrees = 50;
    //     motor::pushHistory(movement);
    //     movement.send();
    //     turnedOnce = true;
    //     if (motor::getIndex() >= 1) {
    //         motor::getOppositeMovement(motor::popHistory()).send();
    //     }
    // }
    // if (time::is_after(10000)) {
    //     // telemetry::send(F("history"), true);
    //     if (motor::getIndex() >= 1) {
    //         motor::Movement movement = motor::getOppositeMovement(motor::popHistory());
    //         telemetry::send(F("motor:degrees_rotation_after"), movement.degrees);
    //         if (movement.degrees != 0) {
    //             motor::rotate_robot(movement.degrees, movement.direction);
    //         } else {
    //             motor::drive_straight(movement.speed, movement.time);
    //         }
    //     }
    // }
    // else if (time::is_after(5000) && time::is_before(8000) && turnedOnce) {
    //     // telemetry::send(F("motor:rotation_angle"), 69);
    //     motor::rotate_robot(140, motor::Direction::LEFT);
    //     motor::Movement movement;
    //     movement.direction = motor::Direction::LEFT;
    //     movement.degrees = 140;
    //     motor::pushHistory(movement);
    //     movement.send();
    //     turnedOnce = false;
    //     if (motor::getIndex() >= 1) {
    //         motor::getOppositeMovement(motor::popHistory()).send();
    //     }
    // }

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
}

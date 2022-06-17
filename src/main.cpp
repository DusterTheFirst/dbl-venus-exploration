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
bool finished;
bool movedOnce;
int movementSpeed = 1430;

void setup() {
    telemetry::init();

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

    turnedOnce = false;
    finished = false;
    movedOnce = false;

    time::init();
    motor::point_ultrasonic(0);
}

#define STEP_BY 1

int8_t heading = -90;
int8_t step = STEP_BY;

uint16_t last_readings[181] = { 0 };

void loop() {
    // Call into command service routines if a command has been received
    telemetry::command::process_command();

    // telemetry::send(F("infrared:raw:cliff:left"), infrared::raw::cliff_left());
    // telemetry::send(F("infrared:raw:cliff:right"), infrared::raw::cliff_right());
    // telemetry::send(F("infrared:raw:rock:left"), infrared::raw::rock_left());
    // telemetry::send(F("infrared:raw:rock:right"), infrared::raw::rock_right());

    // infrared::debug_calibration();

    // Do not overwhelm the serial (FTDI) chip on cheap knock off boards
    delay(200);

    if (!movedOnce) {
        motor::drive_straight(movementSpeed + 15);
        // infrared::detect::cliff_left() || infrared::detect::cliff_right()
        if (ultrasonic::mountain()) {

            motor::stop_motor();
            movedOnce = true;
        }

    } else if (movedOnce) {
        delay(200);
        motor::drive_straight(1540, 500);
        delay(100);
        motor::rotate_robot(180, motor::Direction::LEFT);
        movedOnce = false;
    }

    // if (time::is_before(3500) && !movedOnce) {
    //     motor::drive_straight(movementSpeed, 2000);
    //     motor::Movement movement;
    //     motor::Movement::Forward forward;
    //     forward.speed = movementSpeed;
    //     forward.time = 2000;
    //     movement.type = movement.FORWARD;
    //     movement.value.forward = forward;
    //     motor::push_history(movement);
    //     movedOnce = true;
    // } else if (time::is_after(3500) && time::is_before(4200) && movedOnce) {

    //     motor::Movement movement2;
    //     motor::rotate_robot(90, motor::Direction::LEFT);
    //     motor::Movement::Rotation rotation;
    //     movement2.type = movement2.ROTATION;
    //     rotation.direction = motor::Direction::LEFT;
    //     rotation.degrees = 90;
    //     movement2.value.rotation = rotation;
    //     motor::push_history(movement2);
    //     // movement2.send();

    //     movedOnce = false;
    // } else if (time::is_after(4200) && time::is_before(9000) && !movedOnce) {
    //     motor::drive_straight(movementSpeed, 1200);
    //     motor::Movement movement;
    //     motor::Movement::Forward forward;
    //     forward.speed = movementSpeed;
    //     forward.time = 1200;
    //     movement.type = movement.FORWARD;
    //     movement.value.forward = forward;
    //     motor::push_history(movement);

    //     delay(100);
    //     motor::actuate_grabber(motor::GrabberPosition::OPEN);
    //     delay(300);
    //     motor::drive_straight(1420, 500);
    //     forward.speed = 1420;
    //     forward.time = 500;
    //     movement.type = movement.FORWARD;
    //     movement.value.forward = forward;
    //     motor::push_history(movement);
    //     delay(200);
    //     motor::actuate_grabber(motor::GrabberPosition::CLOSED);
    //     delay(100);

    //     movedOnce = true;
    // } else if (time::is_after(9000) && !turnedOnce && movedOnce) {
    //     if (motor::get_index() >= 1) {
    //         motor::Movement movement = motor::get_opposite_movement(motor::pop_history());
    //         if (movement.type == movement.ROTATION) {
    //             // telemetry::send(F("motor:degrees_rotation_after"), movement.value.rotation.degrees);
    //             motor::rotate_robot(movement.value.rotation.degrees, movement.value.rotation.direction);
    //         } else if (movement.type == movement.FORWARD) {
    //             motor::drive_straight(movement.value.forward.speed, movement.value.forward.time);
    //         }
    //         delay(400);
    //     } else {
    //         turnedOnce = true;
    //     }
    // } else if (time::is_after(13000) && !finished) {

    //     motor::rotate_robot(130, motor::Direction::RIGHT);
    //     delay(200);

    //     motor::drive_straight(movementSpeed - 20, 5500);
    //     motor::actuate_grabber(motor::GrabberPosition::OPEN);
    //     motor::stop_motor();

    //     finished = true;
    // }

    // telemetry::send(F("history"), true);
    //   motor::rotate_robot(50, motor::Direction::RIGHT);

    /*
    if (time::is_before(5000) && !turnedOnce) {
        motor::drive_straight(movementSpeed, 2000);
        motor::Movement movement;
        motor::Movement::Forward forward;
        forward.speed = movementSpeed;
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
    // pathfinding::random_strategy(1475);
}

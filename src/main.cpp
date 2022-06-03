#include <Arduino.h>

// ----- Include telemetry -----
#include <telemetry.hpp>

// ----- Include drivers -----
#include <gyro.hpp>
#include <infrared.hpp>
#include <motor.hpp>
#include <ultrasonic.hpp>

// ----- Include pathfinding -----
#include <pathfinding.hpp>

bool turnedOnce;
uint32_t start;

void setup() {
    telemetry::init();

    telemetry::send(F("main:initializing"), true);
    telemetry::send(F("main:running"), false);

    gyro::init();
    delay(3000);
    motor::init();
    infrared::init();

    telemetry::send(F("main:initializing"), false);
    telemetry::send(F("main:running"), true);

    start = millis();
    turnedOnce = false;
    // motor::rotate_robot(90, motor::Direction::LEFT);
    // delay(5000);
    // motor::rotate_robot(180, motor::Direction::RIGHT);
}

#define STEP_BY 1

int8_t heading = -90;
int8_t step = STEP_BY;

uint16_t last_readings[181] = { 0 };

void loop() {
    // for (float speed = -1.0f; speed <= 1.0f; speed += 0.1f) {
    //     motor::drive_straight(speed, 0);
    // }

    // motor::point_ultrasonic(heading);

    // // uint16_t distance = ultrasonic::distance();
    // uint16_t distance = ultrasonic::mock_distance();

    // telemetry::send(F("ultrasonic:distance"), distance);
    // last_readings[heading + 90] = distance;

    // if (heading % 5 == 0) {
    //     telemetry::send_arr(F("ultrasonic:last_readings"), last_readings,
    //                         sizeof(last_readings) / sizeof(last_readings[0]));
    // }

    // if (heading >= 90) {
    //     step = -STEP_BY;
    // } else if (heading <= -90) {
    //     step = STEP_BY;
    // }

    // heading += step;
    // telemetry::send(F)

    if (millis() - start < 6000 && !turnedOnce) {
        motor::rotate_robot(50, motor::Direction::RIGHT);
        turnedOnce = true;
    } else if ((6000 <= (millis() - start) && (millis() - start) < 16000) && turnedOnce) {
        // telemetry::send(F("motor:rotation_angle"), 69);
        motor::rotate_robot(140, motor::Direction::LEFT);
        turnedOnce = false;
    }

    telemetry::send(F("test:gyro_angle"), gyro::get_angle());
}
#include <Arduino.h>

// ----- Include telemetry -----
#include <telemetry.hpp>

// ----- Include drivers -----
#include <infrared.hpp>
#include <motor.hpp>
#include <ultrasonic.hpp>

// ----- Include pathfinding -----
#include <pathfinding.hpp>

void setup() {
    telemetry::init();

    telemetry::send(F("main:initializing"), true);
    telemetry::send(F("main:running"), false);

    motor::init();
    infrared::init();

    telemetry::send(F("main:initializing"), false);
    telemetry::send(F("main:running"), true);
}

#define STEP_BY 1

int8_t heading = -90;
int8_t step = STEP_BY;

uint16_t last_readings[181] = { 0 };

void loop() {
    // for (float speed = -1.0f; speed <= 1.0f; speed += 0.1f) {
    //     motor::drive_straight(speed, 0);
    // }

    motor::point_ultrasonic(heading);
    uint16_t distance = ultrasonic::distance();
    // uint64_t distance = random(0, 301);
    telemetry::send(F("ultrasonic:distance"), distance);
    last_readings[heading + 90] = distance;

    if (heading % 5 == 0) {
        telemetry::send_arr(F("ultrasonic:last_readings"), last_readings,
                            sizeof(last_readings) / sizeof(last_readings[0]));
    }

    if (heading >= 90) {
        step = -STEP_BY;
    } else if (heading <= -90) {
        step = STEP_BY;
    }

    heading += step;

    // delay(100);
}

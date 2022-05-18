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

    telemetry::send("main:initializing", true);
    telemetry::send("main:running", false);

    motor::init();
    infrared::init();

    telemetry::send("main:initializing", false);
    telemetry::send("main:running", true);
}

int8_t heading = -90;
int8_t step = 1;

void loop() {
    // for (float speed = -1.0f; speed <= 1.0f; speed += 0.1f) {
    //     motor::drive_straight(speed, 0);
    // }

    motor::point_ultrasonic(heading);
    telemetry::send("ultrasonic:distance", ultrasonic::distance());

    heading += step;

    if (heading > 90) {
        step = -1;
    } else if (heading < -90) {
        step = 1;
    }
}

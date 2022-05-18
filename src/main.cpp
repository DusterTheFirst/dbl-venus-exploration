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

void loop() {
    motor::point_ultrasonic(0);

    // for (float speed = -1.0f; speed <= 1.0f; speed += 0.1f) {
    //     motor::drive_straight(speed, 0);
    // }

    // int64_t test_distance = 200;
    telemetry::send("ultrasonic:distance", ultrasonic::distance());

    // for (int8_t heading = -90; heading <= 90; heading += 5) {
    // motor::point_ultrasonic(heading);
    // delay(50);
    // telemetry::send("ultrasonic:distance", ultrasonic::distance());
    // telemetry::send("ultrasonic:distance", 300);
    // test_distance += random(-10, 10);
    // if (test_distance > 300) test_distance = 300;
    // if (test_distance < 0) test_distance = 0;

    // telemetry::send("ultrasonic:distance", test_distance); // FIXME: test
    // }

    delay(1000);
}

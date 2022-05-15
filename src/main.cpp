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

    pinMode(LED_BUILTIN, OUTPUT);

    // motor::init();
    // infrared::init();

    telemetry::send("main:initializing", false);
    telemetry::send("main:running", true);
}

void loop() {
    // telemetry::send("main:size_of_bool", sizeof(bool));
    telemetry::send("main:millis", millis());
    telemetry::send("main:micros", micros());
    // telemetry::send("main:super_long_name_with_an_ultra_wide_metric",
    // (uint64_t)0);

    // for (float speed = -1.0f; speed <= 1.0f; speed += 0.1f) {
    //     motor::drive_straight(speed);
    // }

    bool led = digitalRead(LED_BUILTIN);
    telemetry::send("main:led", led);
    digitalWrite(LED_BUILTIN, !led);

    // telemetry::send("ultrasonic:distance", ultrasonic::distance());

    delay(100);
}

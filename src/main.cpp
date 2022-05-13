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
    motor::init();
    infrared::init();
}

void loop() {
    telemetry::send("main:initialized", true);
    telemetry::send("main:millis", millis());
    telemetry::send("main:micros", micros());
    telemetry::send("main:zero", (uint8_t)0);

    for (float speed = -1.0; speed <= 1.0; speed += 0.1) {
        //  motor::drive_straight(speed); disable because it was cringe
    }

    delay(1000);
}

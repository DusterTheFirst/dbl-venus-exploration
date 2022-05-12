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
}

void loop() {
    telemetry::send("main:initialized", true);
    telemetry::send("main:millis", (uint32_t)millis());
    telemetry::send("main:micros", (uint32_t)micros());
    telemetry::send("main:zero", (uint8_t)0);

    delay(1000);
}

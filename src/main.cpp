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

    telemetry::send(F("initialization"), true);
    // put your setup code here, to run once:
}

void loop() {
    // put your main code here, to run repeatedly:
}

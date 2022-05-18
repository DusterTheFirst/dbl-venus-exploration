#include <Arduino.h>
#include <ultrasonic.hpp>

// this constant won't change. It's the pin number of the sensor's output:
#define PING_PIN 9

uint64_t microsecondsToCentimeters(int64_t microseconds) {
    // The speed of sound is 340 m/s or 29 microseconds per centimeter.
    // The ping travels out and back, so to find the distance of the object we
    // take half of the distance travelled.
    return microseconds / 29 / 2;
}

uint64_t ultrasonic::distance() {
    // The PING))) is triggered by a HIGH pulse of 2 or more microseconds.
    // Give a short LOW pulse beforehand to ensure a clean HIGH pulse:
    pinMode(PING_PIN, OUTPUT);
    digitalWrite(PING_PIN, LOW);
    delayMicroseconds(2);
    digitalWrite(PING_PIN, HIGH);
    delayMicroseconds(5);
    digitalWrite(PING_PIN, LOW);

    // The same pin is used to read the signal from the PING))): a HIGH pulse
    // whose duration is the time (in microseconds) from the sending of the ping
    // to the reception of its echo off of an object.
    pinMode(PING_PIN, INPUT);

    // Do not disable interrupts, as it fucks with the Servo.h library
    // noInterrupts();
    uint64_t duration = pulseIn(PING_PIN, HIGH);
    // interrupts();

    return microsecondsToCentimeters(duration);
}

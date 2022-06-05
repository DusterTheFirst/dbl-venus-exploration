#include <Arduino.h>
#include <EEPROM.h>

#include <infrared.hpp>
#include <telemetry.hpp>

#define CLIFF_SENSOR_1 A5
#define CLIFF_SENSOR_2 A4

#define ROCK_SENSOR_1 A2
#define ROCK_SENSOR_2 A1

#define CALIBRATION_ADDRESS 0

struct Calibration {
    uint16_t reference;
    uint16_t ambient;
};

struct IRCalibration {
    Calibration cliff;
    Calibration rock;
};

static IRCalibration calibration;

void infrared::init() {
    analogReference(DEFAULT);

    pinMode(CLIFF_SENSOR_1, INPUT);
    pinMode(CLIFF_SENSOR_2, INPUT);
    pinMode(ROCK_SENSOR_1, INPUT);
    pinMode(ROCK_SENSOR_2, INPUT);

    // Load calibration data from the EEPROM
    EEPROM.get(CALIBRATION_ADDRESS, calibration);

    telemetry::send(F("infrared:calibration"), calibration);
}

uint16_t infrared::test() {
    return (uint16_t)analogRead(CLIFF_SENSOR_1);
}

infrared::RobotSides infrared::sees_edge() {
    // TODO: detect edge on all four sides of the robot

    return {
        // TODO: Replace these constants with the actual calculated
        // values. These exist just to show how you would construct
        // the RobotSides structure
        .left = false,
        .right = false,
        .front = false,
        .back = false,
    };
}

infrared::RobotSides infrared::sees_rock() {
    // TODO: detect rock on all four sides of the robot

    return {
        // TODO: Replace these constants with the actual calculated
        // values. These exist just to show how you would construct
        // the RobotSides structure
        .left = false,
        .right = false,
        .front = false,
        .back = false,
    };
}

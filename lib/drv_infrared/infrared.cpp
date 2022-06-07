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

    telemetry::send_arr(F("infrared:calibration"), (uint16_t *)&calibration,
                        sizeof(calibration) / sizeof(uint16_t));
}

uint16_t infrared::test_raw() {
    return (uint16_t)analogRead(CLIFF_SENSOR_1);
}

bool infrared::test_detect_rock(int16_t *margin) {
    return false;
}

bool infrared::test_detect_cliff(int16_t *margin) {
    return false;
}

void infrared::calibrate_ambient() {
    telemetry::send(F("infrared:calibrating.ambient"), true);
    telemetry::send(F("infrared:calibrating.ambient"), false);
}

void infrared::calibrate_reference() {
    telemetry::send(F("infrared:calibrating.reference"), true);
    telemetry::send(F("infrared:calibrating.reference"), false);
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

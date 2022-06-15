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

    inline uint16_t quartile_width() {
        return (max(reference, ambient) - min(reference, ambient)) / 4;
    }

    inline bool above_ambient() {
        return reference > ambient;
    }

    inline uint16_t on_thresh() {
        if (this->above_ambient()) {
            return ambient + this->quartile_width() * 3;
        } else {
            return ambient - this->quartile_width() * 3;
        }
    }

    inline uint16_t off_thresh() {
        if (this->above_ambient()) {
            return ambient + this->quartile_width() * 2;
        } else {
            return ambient - this->quartile_width() * 2;
        }
    }

    inline bool test(uint16_t reading) {
        static bool previous_state = false;

        // Schmitt Trigger
        if (previous_state) {
            // Check if crossed off threshold
            if ((this->above_ambient() && reading < this->off_thresh()) ||
                (!this->above_ambient() && reading > this->off_thresh())) {
                previous_state = false;
                return false;
            } else {
                return true;
            }
        } else {
            // Check if crossed on threshold
            if ((this->above_ambient() && reading > this->on_thresh()) ||
                (!this->above_ambient() && reading < this->on_thresh())) {
                previous_state = true;
                return true;
            } else {
                return false;
            }
        }
    }
};

struct IRCalibration {
    Calibration cliff;
    Calibration rock;
};

static IRCalibration calibration;

void load_calibration(void) {
    // Load calibration data from the EEPROM
    EEPROM.get(CALIBRATION_ADDRESS, calibration);

    telemetry::send_arr(F("infrared:calibration"), (uint16_t *)&calibration,
                        sizeof(calibration) / sizeof(uint16_t));
}

void store_calibration(void) {
    telemetry::send_arr(F("infrared:calibration"), (uint16_t *)&calibration,
                        sizeof(calibration) / sizeof(uint16_t));

    // Store calibration data into the EEPROM
    EEPROM.put(CALIBRATION_ADDRESS, calibration);
}

void infrared::init(void) {
    analogReference(DEFAULT);

    pinMode(CLIFF_SENSOR_1, INPUT);
    pinMode(CLIFF_SENSOR_2, INPUT);
    pinMode(ROCK_SENSOR_1, INPUT);
    pinMode(ROCK_SENSOR_2, INPUT);

    load_calibration();
}

uint16_t infrared::test_raw(void) {
    return (uint16_t)analogRead(CLIFF_SENSOR_1);
}

bool infrared::test_detect_rock(void) {
    uint16_t reference = calibration.rock.reference;
    uint16_t ambient = calibration.rock.ambient;

    telemetry::send(F("infrared:calibration.reference"), reference);
    telemetry::send(F("infrared:calibration.ambient"), ambient);

    uint16_t on_thresh = calibration.rock.on_thresh();
    uint16_t off_thresh = calibration.rock.off_thresh();

    telemetry::send(F("infrared:test.on_thresh"), on_thresh);
    telemetry::send(F("infrared:test.off_thresh"), off_thresh);

    return calibration.rock.test(infrared::test_raw());
}

// Need to return cliff detection for L/R -> Struct?
bool infrared::test_detect_cliff(void) {
    return false;
}

void infrared::calibrate_ambient(void) {
    telemetry::send(F("infrared:calibrating.ambient"), true);

    // Take an incremental average for an amount of time, and store in EEPROM
    float average = 0.0;
    for (uint32_t samples = 1; samples < 65535; samples++) {
        if (samples % 64 == 0) {
            telemetry::send(F("infrared:ambient.rock"), average);
        }

        // https://math.stackexchange.com/questions/106700/incremental-averaging
        average += ((float)infrared::test_raw() - average) / (float)samples;
        // TODO: FIXME: compound readings from multiple sensors or save
        // calibration per sensor
    }

    calibration.rock.ambient = (uint16_t)round(average);

    telemetry::send(F("infrared:calibrating.ambient"), false);

    store_calibration();
}

void infrared::calibrate_reference(void) {
    telemetry::send(F("infrared:calibrating.reference"), true);

    // Take an incremental average for an amount of time, and store in EEPROM
    float average = 0.0;
    for (uint32_t samples = 1; samples < 65535; samples++) {
        if (samples % 64 == 0) {
            telemetry::send(F("infrared:reference.rock"), average);
        }

        // https://math.stackexchange.com/questions/106700/incremental-averaging
        average += ((float)infrared::test_raw() - average) / (float)samples;
    }

    calibration.rock.reference = (uint16_t)round(average);

    telemetry::send(F("infrared:calibrating.reference"), false);

    store_calibration();
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

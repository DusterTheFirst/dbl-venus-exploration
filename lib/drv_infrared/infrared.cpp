#include <Arduino.h>
#include <EEPROM.h>

#include <infrared.hpp>
#include <telemetry.hpp>

#define CLIFF_SENSOR_RIGHT A3
#define CLIFF_SENSOR_LEFT A1

#define ROCK_SENSOR_RIGHT A2
#define ROCK_SENSOR_LEFT A0

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

    inline void debug(void) {
        telemetry::send(F("infrared:cliff:ambient"), cliff.ambient);
        telemetry::send(F("infrared:cliff:reference"), cliff.reference);
        telemetry::send(F("infrared:cliff:on_thresh"), cliff.on_thresh());
        telemetry::send(F("infrared:cliff:off_thresh"), cliff.off_thresh());

        telemetry::send(F("infrared:rock:ambient"), rock.ambient);
        telemetry::send(F("infrared:rock:reference"), rock.reference);
        telemetry::send(F("infrared:rock:on_thresh"), rock.on_thresh());
        telemetry::send(F("infrared:rock:off_thresh"), rock.off_thresh());
    }
};

static IRCalibration calibration;

void load_calibration(void) {
    // Load calibration data from the EEPROM
    EEPROM.get(CALIBRATION_ADDRESS, calibration);

    calibration.debug();
}

void store_calibration(void) {
    // Store calibration data into the EEPROM
    EEPROM.put(CALIBRATION_ADDRESS, calibration);

    calibration.debug();
}

namespace infrared {
    void init(void) {
        analogReference(DEFAULT);

        pinMode(CLIFF_SENSOR_LEFT, INPUT);
        pinMode(CLIFF_SENSOR_RIGHT, INPUT);
        pinMode(ROCK_SENSOR_LEFT, INPUT);
        pinMode(ROCK_SENSOR_RIGHT, INPUT);

        load_calibration();
    }

    void debug_calibration(void) {
        calibration.debug();
    }

    namespace raw {
        uint16_t cliff_left(void) {
            return (uint16_t)analogRead(CLIFF_SENSOR_LEFT);
        }

        uint16_t cliff_right(void) {
            return (uint16_t)analogRead(CLIFF_SENSOR_RIGHT);
        }

        uint16_t rock_left(void) {
            return (uint16_t)analogRead(ROCK_SENSOR_LEFT);
        }

        uint16_t rock_right(void) {
            return (uint16_t)analogRead(ROCK_SENSOR_RIGHT);
        }
    }

    namespace detect {
        bool cliff_left(void) {
            return calibration.cliff.test(raw::cliff_left());
        }

        bool cliff_right(void) {
            return calibration.cliff.test(raw::cliff_right());
        }

        bool rock_left(void) {
            return calibration.rock.test(raw::rock_left());
        }

        bool rock_right(void) {
            return calibration.rock.test(raw::rock_right());
        }
    }

    void generic_calibrate(uint16_t *const rock, uint16_t *const cliff) {
        // Take an incremental average for an amount of time
        // and store in EEPROM
        // https://math.stackexchange.com/questions/106700/incremental-averaging

        float rock_average = 0.0;
        float cliff_average = 0.0;
        for (uint32_t samples = 1; samples < 65535; samples++) {
            // Do the rock
            {
                uint16_t rock_left_sample = infrared::raw::rock_left();
                uint16_t rock_right_sample = infrared::raw::rock_right();

                float rock_sample_average = ((float)rock_left_sample +
                                             (float)rock_right_sample) /
                                            2.0;

                rock_average += (rock_sample_average - rock_average) /
                                (float)samples;
            }

            // Do the cliff
            {
                uint16_t cliff_left_sample = infrared::raw::cliff_left();
                uint16_t cliff_right_sample = infrared::raw::cliff_right();

                float cliff_sample_average = ((float)cliff_left_sample +
                                              (float)cliff_right_sample) /
                                             2.0;

                cliff_average += (cliff_sample_average - cliff_average) /
                                 (float)samples;
            }

            if (samples % 64 == 0) {
                telemetry::send(F("infrared:calibrating:rock"),
                                rock_average);
                telemetry::send(F("infrared:calibrating:cliff"),
                                cliff_average);
            }
        }

        *rock = (uint16_t)round(rock_average);
        *cliff = (uint16_t)round(cliff_average);
    }

    namespace calibrate {
        void ambient(void) {
            telemetry::send(F("infrared:calibrating:ambient"), true);

            generic_calibrate(&calibration.rock.ambient,
                              &calibration.cliff.ambient);

            telemetry::send(F("infrared:calibrating:ambient"), false);

            store_calibration();
        }

        void reference(void) {
            telemetry::send(F("infrared:calibrating:reference"), true);

            generic_calibrate(&calibration.rock.reference,
                              &calibration.cliff.reference);

            telemetry::send(F("infrared:calibrating:reference"), false);

            store_calibration();
        }
    }
}

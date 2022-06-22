#include <Arduino.h>

// ----- Include telemetry -----
#include <command.hpp>
#include <telemetry.hpp>

// ----- Include drivers -----
#include <gyro.hpp>
#include <infrared.hpp>
#include <motor.hpp>
#include <ultrasonic.hpp>

// ----- Include pathfinding -----
#include <pathfinding.hpp>

// ----- Include timekeeping -----
#include <time.h>

void setup() {
    telemetry::init();

    // Give the serial port time to connect
    delay(1000);

    telemetry::command::load_vector_table({
        .CalibrateAmbientInfrared = infrared::calibrate::ambient,
        .CalibrateReferenceInfrared = infrared::calibrate::reference,
    });

    telemetry::send(F("main:initializing"), true);
    telemetry::send(F("main:running"), false);

    infrared::init();

    telemetry::send(F("main:initializing"), false);
    telemetry::send(F("main:running"), true);

    time::init();
}

void loop() {
    // Call into command service routines if a command has been received
    telemetry::command::process_command();

    telemetry::send(F("infrared:raw:cliff:left"),
                    infrared::raw::cliff_left());
    telemetry::send(F("infrared:raw:cliff:right"),
                    infrared::raw::cliff_right());
    telemetry::send(F("infrared:raw:rock:left"),
                    infrared::raw::rock_left());
    telemetry::send(F("infrared:raw:rock:right"),
                    infrared::raw::rock_right());

    telemetry::send(F("infrared:detect:cliff:left"),
                    infrared::detect::cliff_left());
    telemetry::send(F("infrared:detect:cliff:right"),
                    infrared::detect::cliff_right());
    telemetry::send(F("infrared:detect:rock:left"),
                    infrared::detect::rock_left());
    telemetry::send(F("infrared:detect:rock:right"),
                    infrared::detect::rock_right());

    infrared::debug_calibration();

    // Do not overwhelm the serial (FTDI) chip on cheap knock off boards
    delay(16);
}

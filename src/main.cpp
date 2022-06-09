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
        .CalibrateAmbientInfrared = infrared::calibrate_ambient,
        .CalibrateReferenceInfrared = infrared::calibrate_reference,
    });

    telemetry::send(F("main:initializing"), true);
    telemetry::send(F("main:running"), false);

    // gyro::init();
    // delay(3000);
    // motor::init();
    infrared::init();

    telemetry::send(F("main:initializing"), false);
    telemetry::send(F("main:running"), true);

    // motor::rotate_robot(50, motor::Direction::RIGHT);
    // delay(5000);
    // motor::rotate_robot(140, motor::Direction::LEFT);

    time::init();
}

#define STEP_BY 1

int8_t heading = -90;
int8_t step = STEP_BY;

uint16_t last_readings[181] = { 0 };

void loop() {
    // Call into command service routines if a command has been received
    telemetry::command::process_command();

    uint16_t test = infrared::test_raw();
    float voltage = ((float)test / (float)(1 << 10)) * 5.0;
    telemetry::send(F("infrared:test"), test);
    telemetry::send(F("infrared:voltage"), voltage);

    telemetry::send(F("infrared:rock"), infrared::test_detect_rock());
    telemetry::send(F("infrared:cliff"), infrared::test_detect_cliff());

    delay(100);

    // for (float speed = -1.0f; speed <= 1.0f; speed += 0.1f) {
    //     motor::drive_straight(speed, 0);
    // }

    // motor::point_ultrasonic(heading);

    // // uint16_t distance = ultrasonic::distance();
    // uint16_t distance = ultrasonic::mock_distance();

    // telemetry::send(F("ultrasonic:distance"), distance);
    // last_readings[heading + 90] = distance;

    // if (heading % 5 == 0) {
    //     telemetry::send_arr(F("ultrasonic:last_readings"), last_readings,
    //                    sizeof(last_readings) / sizeof(last_readings[0]));
    // }

    // if (heading >= 90) {
    //     step = -STEP_BY;
    // } else if (heading <= -90) {
    //     step = STEP_BY;
    // }

    // heading += step;
    // telemetry::send(F)
}

#include "gyro.hpp"
#include <Arduino.h>
#include <I2Cdev.h>
#include <MPU6050_6Axis_MotionApps20.h>
#if I2CDEV_IMPLEMENTATION == I2CDEV_ARDUINO_WIRE
#include "Wire.h"
#include "telemetry.hpp"
#endif

MPU6050 mpu;

// MPU control/status vars
bool dmpReady = false;  // set true if DMP init was successful
uint8_t devStatus;      // return status after each device operation
                        // (0 = success, !0 = error)
uint16_t packetSize;    // expected DMP packet size (default is 42 bytes)
uint16_t fifoCount;     // count of all bytes currently in FIFO
uint8_t fifoBuffer[64]; // FIFO storage buffer

// Orientation / motion vars
Quaternion q;        // [w, x, y, z]         quaternion container
VectorFloat gravity; // [x, y, z]            gravity vector
float ypr[3];        // [yaw, pitch, roll]   yaw/pitch/roll container
                     // and gravity vector
int angle;
int current_angle = -200;
int motor_modifier = -8;

void gyro::init() {
// join I2C bus (I2Cdev library doesn't do this automatically)
#if I2CDEV_IMPLEMENTATION == I2CDEV_ARDUINO_WIRE
    Wire.begin();
    Wire.setClock(400000); // 400kHz I2C clock.
#elif I2CDEV_IMPLEMENTATION == I2CDEV_BUILTIN_FASTWIRE
    Fastwire::setup(400, true);
#endif

    // Init connection
    telemetry::send(F("gyro:initialized"), false);
    mpu.initialize();
    telemetry::send(F("gyro:initialized"), true);

    // verify connection
    telemetry::send(F("gyro::connected"), mpu.testConnection());

    initMPU();
}

void gyro::initMPU() {
    // load and configure the DMP
    telemetry::send(F("gyro:dmp.initialized"), false);
    devStatus = mpu.dmpInitialize();

    // Gyro offset
    mpu.setXGyroOffset(220);
    mpu.setYGyroOffset(76);
    mpu.setZGyroOffset(-85);
    mpu.setZAccelOffset(1788); // 1688 factory default

    if (devStatus == 0) {
        telemetry::send(F("gyro:dmp.initialized"), true);

        // Calibration Time: MPU6050
        mpu.CalibrateAccel(6);
        mpu.CalibrateGyro(6);
        mpu.PrintActiveOffsets();
        // turn on the DMP, now that it's ready
        telemetry::send(F("gyro:dmp.enabled"), false);
        mpu.setDMPEnabled(true);
        telemetry::send(F("gyro:dmp.enabled"), true);

        dmpReady = true;

        // get expected DMP packet size for later comparison
        packetSize = mpu.dmpGetFIFOPacketSize();
    } else {
        // ERROR!
        // 1 = initial memory load failed
        // 2 = DMP configuration updates failed
        // (if it's going to break, usually the code will be 1)
        telemetry::send(F("gyro:dmp.error_ode"), devStatus);
    }
}

int16_t gyro::get_angle() {
    // wait until the buffer has a value for the gyro
    do {
        delayMicroseconds(5);
    } while (!mpu.dmpGetCurrentFIFOPacket(fifoBuffer) || !dmpReady);

    // display Euler angles in degrees
    mpu.dmpGetQuaternion(&q, fifoBuffer);
    mpu.dmpGetGravity(&gravity, &q);
    mpu.dmpGetYawPitchRoll(ypr, &q, &gravity);

    telemetry::send(F("gyro:angle"), ypr[0] * 180 / M_PI);

    return ypr[0] * 180 / M_PI;
}

#include <Arduino.h>

#include <telemetry.hpp>

COBSPacketSerial packet_serial;

void telemetry::init() {
    packet_serial.begin(9600);
}

void telemetry::__send(
    const char *metric_name,
    const char *metric_type,
    const uint8_t *metric,
    const size_t metric_size) {
    // Send the packet header
    Serial.write(0x00000000);
    // Send the metric name
    Serial.write(metric_name);
    // Send the metric type
    Serial.write(metric_type);
    // Send the metric itself with COBS encoding
    packet_serial.send(metric, metric_size);
}

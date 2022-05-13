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
    static const uint8_t header[4] = {0x00, 0x00, 0x00, 0x00};
    // Send the packet header
    Serial.write(header, 4);
    // Send the metric name
    Serial.write(metric_name, strlen(metric_name) + 1);
    // Send the metric type
    Serial.write(metric_type, strlen(metric_type) + 1);
    // Send the metric itself with COBS encoding
    packet_serial.send(metric, metric_size);
}

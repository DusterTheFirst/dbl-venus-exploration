#include <Arduino.h>

#include <telemetry.hpp>
#include <cobs.hpp>

void telemetry::init() {
    Serial.begin(115200);
}

#define PACKET_BUFFER_LEN 420

uint8_t packet_buffer[PACKET_BUFFER_LEN];

void telemetry::__send(
    const __FlashStringHelper *metric_name_fsh,
    const __FlashStringHelper *metric_type_fsh,
    const uint8_t *metric,
    const size_t metric_size) {
    // This is not always guaranteed cause C is a good programming language :)
    static_assert(sizeof(uint8_t) == sizeof(char));

    // Reinterpret the __FlashStringHelper pointers to char pointers
    const char *metric_name = reinterpret_cast<const char *>(metric_name_fsh);
    const char *metric_type = reinterpret_cast<const char *>(metric_type_fsh);

    size_t metric_name_len = strlen_P(metric_name) + 1;
    size_t metric_type_len = strlen_P(metric_type) + 1;

    size_t packet_length = sizeof(uint32_t) +
                           metric_type_len + metric_name_len + metric_size +
                           sizeof(size_t);

    if (packet_length > PACKET_BUFFER_LEN) {
        // packet too large, drop
        return; // TODO: report error somehow
    }

    // Construct the packet
    {
        size_t bytes_written = 0;

        uint32_t timestamp = millis();

        // timestamp
        memcpy(&packet_buffer[bytes_written],
               (const uint8_t *)&timestamp, sizeof(uint32_t));
        bytes_written += sizeof(uint32_t);

        // metric name
        memcpy_P(&packet_buffer[bytes_written],
                 (const uint8_t *)metric_name, metric_name_len);
        bytes_written += metric_name_len;

        // metric type
        memcpy_P(&packet_buffer[bytes_written],
                 (const uint8_t *)metric_type, metric_type_len);
        bytes_written += metric_type_len;

        // metric
        memcpy(&packet_buffer[bytes_written], metric, metric_size);
        bytes_written += metric_size;

        // length of the raw packet
        memcpy(&packet_buffer[bytes_written],
               (uint8_t *)&packet_length, sizeof(size_t));
        bytes_written += sizeof(size_t);
    }

    // Send the COBS encoded message
    telemetry::cobs::serial_write(packet_buffer, packet_length);
}

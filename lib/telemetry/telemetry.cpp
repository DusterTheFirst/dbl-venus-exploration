#include <Arduino.h>

#include <telemetry.hpp>

void telemetry::init() {
    Serial.begin(9600);
}

void telemetry::__send(
    const char *metric_name,
    const char *metric_type,
    const uint8_t *metric,
    const size_t metric_size) {
    // This is not always guaranteed cause C is a good programming language :)
    static_assert(sizeof(uint8_t) == sizeof(char));

    size_t metric_name_len = (strlen(metric_name) + 1);
    size_t metric_type_len = (strlen(metric_type) + 1);

    size_t packet_length = sizeof(uint32_t) +
                           metric_type_len + metric_name_len + metric_size +
                           sizeof(size_t);
    uint8_t *packet_buffer = (uint8_t *)malloc(packet_length);

    if (packet_buffer == NULL) {
        // OOM, drop this packet
        return;
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
        memcpy(&packet_buffer[bytes_written],
               (const uint8_t *)metric_name, metric_name_len);
        bytes_written += metric_name_len;

        // metric type
        memcpy(&packet_buffer[bytes_written],
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

    // Allocate the COBS buffer
    size_t cobs_buffer_length = COBS::getEncodedBufferSize(packet_length);
    uint8_t *cobs_buffer = (uint8_t *)malloc(cobs_buffer_length);

    if (cobs_buffer == NULL) {
        // OOM, drop this packet
        free(packet_buffer);
        return;
    }

    // Encode the packet with COBS
    size_t bytes_written = COBS::encode(packet_buffer, packet_length,
                                        cobs_buffer);

    // Add null byte packet delimeter
    cobs_buffer[bytes_written] = 0;
    bytes_written += 1;

    // Send the COBS encoded message
    Serial.write(cobs_buffer, bytes_written);

    // Free the COBS encoded data and the packet
    free(packet_buffer);
    free(cobs_buffer);
}

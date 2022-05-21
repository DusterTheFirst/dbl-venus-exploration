#include <Arduino.h>

#include "cobs.hpp"
#include "telemetry.hpp"

void telemetry::init() {
    Serial.begin(115200);
}

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

    const uint32_t timestamp = millis();

    // Construct the packet
    chunk::Chunk chunk_arr[] = {
        // timestamp
        chunk::Chunk(false, (const uint8_t *)&timestamp, sizeof(uint32_t)),
        // metric name
        chunk::Chunk(true, (const uint8_t *)metric_name, metric_name_len),
        // metric type
        chunk::Chunk(true, (const uint8_t *)metric_type, metric_type_len),
        // metric
        chunk::Chunk(false, metric, metric_size),
        // length of the raw packet
        chunk::Chunk(false, (uint8_t *)&packet_length, sizeof(size_t)),
    };

    chunk::Chunks chunks(chunk_arr);

    // Send the COBS encoded message
    telemetry::cobs::serial_write(chunks);
}

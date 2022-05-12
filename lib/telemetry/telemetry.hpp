#pragma once

#include <PacketSerial.h>
#include <stdint.h>

/**
 * @brief
 *
 */
namespace telemetry {
    /**
     * @brief Initialize the telemetry subsystem
     *
     * Initialize the serial port
     */
    void init();

    void __send(
        const char *field_name,
        const char *packet_type,
        const uint8_t *packet,
        const size_t packet_length);

    /**
     * @brief Send a metric to a telemetry subscriber
     *
     * @tparam T Packet Type
     * @param name Name of the metric being recorded
     * @param metric The metric being reported
     */
    template <typename T>
    inline static void send(const char *name, T metric);

#define SEND_IMPL(CTYPE, RTYPE)                               \
    inline static void send(const char *name, CTYPE metric) { \
        telemetry::__send(                                    \
            name,                                             \
            RTYPE,                                            \
            (const uint8_t *)&metric,                         \
            sizeof(metric));                                  \
    }

    SEND_IMPL(uint8_t, "u8")
    SEND_IMPL(uint16_t, "u16")
    SEND_IMPL(uint32_t, "u32")
    SEND_IMPL(uint64_t, "u64")

    SEND_IMPL(int8_t, "i8")
    SEND_IMPL(int16_t, "i16")
    SEND_IMPL(int32_t, "i32")
    SEND_IMPL(int64_t, "i64")

    SEND_IMPL(bool, "bool")
}

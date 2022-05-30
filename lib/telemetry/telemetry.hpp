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
        const __FlashStringHelper *field_name,
        const __FlashStringHelper *packet_type,
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
    inline static void send(const __FlashStringHelper *name, T metric);

    /**
     * @brief Send an array of metrics to a telemetry subscriber
     *
     * @tparam T Packet Type
     * @param name Name of the metric being recorded
     * @param metrics The metric array being reported
     * @param length The length of the metric array being reported
     */
    template <typename T>
    inline static void send_arr(const __FlashStringHelper *name, T *metrics,
                                size_t length);

#define SEND_IMPL(CTYPE, RTYPE)                                              \
    inline static void send(const __FlashStringHelper *name, CTYPE metric) { \
        telemetry::__send(                                                   \
            name,                                                            \
            F(RTYPE),                                                        \
            (const uint8_t *)&metric,                                        \
            sizeof(metric));                                                 \
    }

#define SEND_ARR_IMPL(CTYPE, RTYPE)                              \
    inline static void send_arr(const __FlashStringHelper *name, \
                                CTYPE *metrics, size_t length) { \
        telemetry::__send(                                       \
            name,                                                \
            F(RTYPE),                                            \
            (const uint8_t *)metrics,                            \
            sizeof(CTYPE) * length);                             \
    }

    SEND_IMPL(uint8_t, "u8")
    SEND_ARR_IMPL(uint8_t, "[u8]")
    SEND_IMPL(uint16_t, "u16")
    SEND_ARR_IMPL(uint16_t, "[u16]")
    SEND_IMPL(uint32_t, "u32")
    SEND_ARR_IMPL(uint32_t, "[u32]")
    SEND_IMPL(uint64_t, "u64")
    SEND_ARR_IMPL(uint64_t, "[u64]")

    SEND_IMPL(int8_t, "i8")
    SEND_ARR_IMPL(int8_t, "[i8]")
    SEND_IMPL(int16_t, "i16")
    SEND_ARR_IMPL(int16_t, "[i16]")
    SEND_IMPL(int32_t, "i32")
    SEND_ARR_IMPL(int32_t, "[i32]")
    SEND_IMPL(int64_t, "i64")
    SEND_ARR_IMPL(int64_t, "[i64]")

    SEND_IMPL(bool, "bool")
    SEND_ARR_IMPL(bool, "[bool]")

    SEND_IMPL(float, "f32")
    SEND_ARR_IMPL(float, "[f32]")
    SEND_IMPL(double, "f32")
    SEND_ARR_IMPL(double, "[f32]")
    SEND_IMPL(long double, "f32")
    SEND_ARR_IMPL(long double, "[f32]")
}

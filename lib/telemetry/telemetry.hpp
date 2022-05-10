#pragma once

#include <PacketSerial.h>

namespace telemetry {
    void init();
    extern COBSPacketSerial packet_serial;

    // Ensure that the width of a character is the same as one byte
    // This is not true on all platforms cause C(++) is a """Good"""
    // language
    static_assert(sizeof(char) == sizeof(uint8_t));

    template <typename T>
    inline void send(const __FlashStringHelper *name, T packet) {
        // Send the telemetry header
        packet_serial.send((const uint8_t *)name, strlen((const char *)name));
        // Send the telemetry struct raw :) (Dont do this in production)
        packet_serial.send((const uint8_t *)&packet, sizeof(T));
    }
}

#include <stdint.h>
#include <telemetry.hpp>

COBSPacketSerial telemetry::packet_serial;

void telemetry::init() {
    packet_serial.begin(115200);
}

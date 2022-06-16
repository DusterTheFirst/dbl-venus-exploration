#include <command.hpp>

static telemetry::command::VectorTable global_vector_table = { NULL };

static const uint8_t VECTOR_TABLE_LENGTH =
    sizeof(telemetry::command::VectorTable) /
    sizeof(telemetry::command::Vector);

void telemetry::command::load_vector_table(
    telemetry::command::VectorTable vector_table) {
    global_vector_table = vector_table;

#if TELEMETRY_COMMAND_DEBUG
    telemetry::send(F("command:vector_table.loaded"), true);
#endif
}

bool telemetry::command::process_command() {
    int32_t byte_or_neg_one = Serial.read();

    // Check if no byte
    if (byte_or_neg_one == -1) {
        return false;
    }

    uint8_t byte = (uint8_t)byte_or_neg_one;

    // Reject vectors outside of the table
    if (byte >= VECTOR_TABLE_LENGTH) {
#if TELEMETRY_COMMAND_DEBUG
        telemetry::send(F("command:vector:invalid"), byte);
#endif

        return false;
    }

    uint8_t index = byte;

#if TELEMETRY_COMMAND_DEBUG
    telemetry::send(F("command:vector:valid"), byte);
#endif

    telemetry::command::Vector vector =
        ((telemetry::command::Vector *)&global_vector_table)[index];

    if (vector == NULL) {
#if TELEMETRY_COMMAND_DEBUG
        telemetry::send(F("command:vector:uninit"), index);
#endif

        return false;
    }

    vector();

    return true;
}

#include "time.hpp"

static uint32_t beginning_of_time;

void time::init() {
    beginning_of_time = millis();
}

bool time::is_before(uint32_t time_in_millis) {
    return millis() - beginning_of_time < time_in_millis;
}

bool time::is_after(uint32_t time_in_millis) {
    return millis() - beginning_of_time > time_in_millis;
}

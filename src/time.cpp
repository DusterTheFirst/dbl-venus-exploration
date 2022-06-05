#include <time.h>

static uint32_t beginning_of_time;

inline void time::init() {
    beginning_of_time = millis();
}

inline bool time::is_before(uint32_t time_in_millis) {
    return millis() - beginning_of_time < time_in_millis;
}

inline bool time::is_after(uint32_t time_in_millis) {
    return millis() - beginning_of_time > time_in_millis;
}
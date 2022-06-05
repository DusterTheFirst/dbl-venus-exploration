#pragma once

#include <Arduino.h>
#include <stdint.h>

/**
 * @brief Helpers for timing.
 *
 * Functions and other tools wrapping the arduino millis and micros that provide
 * easy, repeatable timing
 */
namespace time {
    /**
     * @brief Initialize the timekeeping.
     * 
     * It is best to call this at the end of main so that no time is skipped
     * before the first call to loop()
     */ 
    inline void init();

    inline bool is_before(uint32_t time_in_millis);
    inline bool is_after(uint32_t time_in_millis);
}

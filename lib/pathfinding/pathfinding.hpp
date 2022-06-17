#pragma once

#include <stdint.h>

/**
 * @brief Pathfinding and navigation code
 */
namespace pathfinding {
    // TODO:

    /**
     * @brief Strategy to drive straight until it finds an obstacle.
     *
     */
    void random_strategy(int16_t speed);

    /**
     * @brief Pops items from the movement stack and moves back from the initial position.
     *
     */
    void return_to_lab();

    void grab_rock();
}

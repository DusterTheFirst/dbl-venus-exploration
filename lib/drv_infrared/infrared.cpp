#include <Arduino.h>
#include <infrared.hpp>

void infrared::init() {
    // TODO: Write initialization code
}

infrared::RobotSides infrared::sees_edge() {
    // TODO: detect edge on all four sides of the robot

    return {
        // TODO: Replace these constants with the actual calculated
        // values. These exist just to show how you would construct
        // the RobotSides structure
        .left = false,
        .right = false,
        .front = false,
        .back = false,
    };
}

infrared::RobotSides infrared::sees_rock() {
    // TODO: detect rock on all four sides of the robot

    return {
        // TODO: Replace these constants with the actual calculated
        // values. These exist just to show how you would construct
        // the RobotSides structure
        .left = false,
        .right = false,
        .front = false,
        .back = false,
    };
}

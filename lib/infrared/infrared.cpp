#include <Arduino.h>
#include <infrared.hpp>

void infrared::init() {
    // TODO: Write initialization code
}

RobotSides infrared::sees_edge() {
    // TODO: detect edge on all four sides of the robot

    return ((RobotSides){
        .left = false, // TODO: Replace these constants with the actual calculated
                       // values. These exist just to show how you would construct
                       // the RobotSides structure
        .right = false,
        .front = false,
        .back = false,
    });
}

RobotSides infrared::sees_rock() {
    // TODO: detect rock on all four sides of the robot

    return ((RobotSides){
        .left = false, // TODO: Replace these constants with the actual calculated
                       // values. These exist just to show how you would construct
                       // the RobotSides structure
        .right = false,
        .front = false,
        .back = false,
    });
}
#include <pathfinding.hpp>
#include <ultrasonic.hpp>
#include <motor.hpp>

void avoid_mountain() {
    bool mountain = ultrasonic::mountain();

    if (mountain == true) {
        //direction Right 90 degrees
        motor::rotate_robot(90, motor::Direction::LEFT);
    }
}
// TODO:

#include <infrared.hpp>
#include <motor.hpp>
#include <pathfinding.hpp>
#include <ultrasonic.hpp>

#define BACKWARD_TURN_LB 90
#define BACKWARD_TURN_RB 180
#define TURN_MIN_BOUND 20
#define TURN_MAX_BOUND 160

void avoid_mountain() {
    bool mountain = ultrasonic::mountain();

    if (mountain == true) {
        // direction Right 90 degrees
        motor::rotate_robot(90, motor::Direction::LEFT);
    }
}

void pathfinding::grab_rock() {
        bool left = false;
        motor::stop_motor();
        motor::actuate_grabber(motor::GrabberPosition::OPEN);
        delay(2000);
        motor::start_motor();
        if (infrared::detect::rock_left()) {
            motor::rotate_robot(10, motor::Direction::LEFT);
            left = true;
        }
        else if (infrared::detect::rock_right()) {
             motor::rotate_robot(10, motor::Direction::RIGHT);
        }
        delay(1000);
        motor::drive_straight(1475, 0.75);
        motor::stop_motor();
        delay(1000);
        motor::actuate_grabber(motor::GrabberPosition::CLOSED);
        delay(2000);
        motor::Movement temp;
        temp.value.forward.speed = 1475;
        temp.value.forward.time = 0.75;
        temp = motor::get_opposite_movement(temp);
        motor::drive_straight(temp.value.forward.speed, temp.value.forward.time);
        delay(1000);
        left == true ? motor::rotate_robot(10, motor::Direction::RIGHT) : motor::rotate_robot(10, motor::Direction::LEFT);
        motor::stop_motor();
}

motor::RotatedTo rotate_to_random(int8_t where_to) {
    motor::RotatedTo rotate_to;
    switch (where_to) {
        case 0:
            rotate_to.degrees = random(BACKWARD_TURN_LB, BACKWARD_TURN_RB);
            rotate_to.direction = random(2) ? motor::Direction::LEFT : rotate_to.direction = motor::Direction::RIGHT;
            break;
        case 1:
            rotate_to.degrees = random(TURN_MIN_BOUND, TURN_MAX_BOUND);
            rotate_to.direction = motor::Direction::LEFT;
            break;
        case 2:
            rotate_to.degrees = random(TURN_MIN_BOUND, TURN_MAX_BOUND);
            rotate_to.direction = motor::Direction::RIGHT;
            break;
        default:
            break;
    }
    motor::rotate_robot(rotate_to.degrees, rotate_to.direction);
    return rotate_to;
}

uint8_t get_end_condition() {
    if (infrared::detect::rock_left() || infrared::detect::rock_right()) {
        return 1;
    } else if (infrared::detect::cliff_left() && infrared::detect::cliff_right()) {
        return 4;
    } else if (infrared::detect::cliff_left() || infrared::detect::cliff_right()) {
        return infrared::detect::cliff_left() ? 2 : 3;
    } else if (ultrasonic::distance() < 18) {
        return 5;
    }

    return 0;
}

// Send -1 for no time
void pathfinding::random_strategy(int16_t speed) {
    motor::Movement ret;
    ret.type = ret.FORWARD;
    ret.value.forward.speed = speed;

    motor::drive_straight(speed);

    /* 0 - Default
     * 1 - Rock
     * 2 - Left cliff
     * 3 - Right cliff
     * 4 - Forward cliff
     * 5 - Ultrasound
     */
    uint8_t end_condition = 0;
    uint32_t start_time = millis();
    uint32_t end_time;

    while (true) {

        end_condition = get_end_condition();

        if (end_condition != 0) {
            break;
        }
    }

    end_time = millis() - start_time;
    ret.value.forward.time = end_time;
    start_time = millis();
    push_history(ret);

    if (end_condition == 1) {
        grab_rock();
        return_to_lab();
    } else {
        motor::RotatedTo ret2;
        ret.type = ret.ROTATION;
        if (end_condition <= 3) {
            delay(500);
            end_time = millis() - start_time;
            motor::stop_motor();
            delay(200);
            if (end_condition == 2 && !(infrared::detect::cliff_right())) {
                ret2 = rotate_to_random(2);
            } else if (end_condition == 3 && !(infrared::detect::cliff_left())) {
                ret2 = rotate_to_random(1);
            } else {
                ret2 = rotate_to_random(0);
            }
        } else {
            motor::stop_motor();
            ret2 = rotate_to_random(0);
        }
        ret.value.rotation.degrees = ret2.degrees;
        ret.value.rotation.direction = ret2.direction;
        push_history(ret);
    }
    delay(200);
}

// TODO: the return to lab should be combined in one function that detects the type of movement
void pathfinding::return_to_lab() {

    if (motor::get_index() == 0) {
        return;
    }

    motor::Movement last_movement = motor::get_opposite_movement(motor::pop_history());

    if (last_movement.type == motor::Movement::FORWARD) {
        motor::drive_straight(last_movement.value.forward.speed, last_movement.value.forward.time);
    } else {
        motor::rotate_robot(last_movement.value.rotation.degrees, last_movement.value.rotation.direction);
    }

    delay(500);
}

// void return_to_lab_rotate() {
//     motor::Movement last_rev = motor::get_opposite_movement(motor::pop_history());
//     delay(200);
//     if (motor::get_index() == 0) {
//         return;
//     }
//     return_to_lab();
// }

// TODO:

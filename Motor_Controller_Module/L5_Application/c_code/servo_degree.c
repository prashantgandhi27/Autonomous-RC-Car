/*
 * servo_degree.c
 *
 *  Created on: May 4, 2019
 *      Author: Jay
 */

#include "servo_degree.h"
#include "printf_lib.h"

    float  on_time = 0;

float get_servo_angle(CAR_CONTROL_t * servo) {

    if((servo->MOTOR_STEER_cmd + 90) > 180 && servo->MOTOR_STEER_cmd <= 270)
        on_time = 20;
    else if((servo->MOTOR_STEER_cmd + 90) > 270 && servo->MOTOR_STEER_cmd <= 359.99)
        on_time = 10;
    else
        on_time = 10 + ((servo->MOTOR_STEER_cmd + 90) * (0.05555));
    return on_time;
}

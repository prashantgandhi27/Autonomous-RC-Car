/*
 * c_pwm.cpp
 *
 *  Created on: Apr 17, 2019
 *      Author: halak
 */

#include "c_pwm.h"
#include "lpc_pwm.hpp"
// Assuming UART2 is already initialized
// You may want to also add uart2_init() in C

PWM *lidar_motor;

void init_pwm(unsigned int freq) {
    lidar_motor = new PWM(PWM::pwm6, freq);
}

void set_pwm_value(float percent) {

    lidar_motor->set(percent);
}

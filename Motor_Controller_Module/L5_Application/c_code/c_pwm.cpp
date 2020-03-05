/*
 * c_pwm.cpp
 *
 *  Created on: Apr 14, 2019
 *      Author: Jay
 */

#include "c_pwm.h"
#include "lpc_pwm.hpp"
//#include "printf_lib.h"
// Assuming UART2 is already initialized
// You may want to also add uart2_init() in C

PWM *motor_control;
PWM *servo_control;

void init_pwm(long unsigned int freq) {
    motor_control = new PWM(PWM::pwm1, freq);
    servo_control = new PWM(PWM::pwm2, freq);
}

bool set_pwm_value(pwm_pin_t pwm_pin, float percent) {
    bool flag = false;
    switch(pwm_pin) {
        case motor_1:
            flag = motor_control->set(percent);
            pwm_val = percent;
            break;
        case servo_2:
            flag = servo_control->set(percent);
            break;
    }
    return flag;
}

float get_pwm_value(void) {
return pwm_val;
        }

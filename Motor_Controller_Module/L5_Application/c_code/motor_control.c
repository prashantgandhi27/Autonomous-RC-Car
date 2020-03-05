/*
 * motor_control.c
 *
 *  Created on: Apr 9, 2019
 *      Author: Jay
 */

#include "motor_control.h"
#include "printf_lib.h"
#include "c_pid.h"
#include "servo_degree.h"
#include "c_gpio.h"

#define STEP_UPHILL 0.002
#define STEP_DOWNHILL 0.0004
#define SPEED_LIMIT 16.05
#define STOP 15.00
#define REVERSE 14.2
#define FORWARD_START 15.7

#define NO_STEER 0.00
#define SLIGHT_RIGHT 17.00
#define SLIGHT_LEFT 13.00
#define HARD_RIGHT 19.00
#define HARD_LEFT 11.00

CAR_CONTROL_t drive = {MOTOR_STOP, NO_STEER, 0.00, {0}};
s_pid_t dc = {0};

static uint32_t reverse_count = 0;
static bool reverse_flag = false;

void command_motor(CAR_CONTROL_t *drive_motor, double *cmd_pwm_val) {
    motor_set_led(_p0_29, 0); //Forward LED
    motor_set_led(_p1_19, 0); //Reverse LED
    motor_set_led(_p0_30, 0); //Stop LED

    switch (drive_motor->MOTOR_DRIVE_cmd) {

        case MOTOR_FORWARD:
            reverse_flag = false;
            motor_set_led(_p0_29, 1);
            forward_speed_control(drive_motor, cmd_pwm_val);
            break;

        case MOTOR_STOP:
            reverse_flag = false;
            set_pwm_value(motor_1, STOP);
            motor_set_led(_p0_30, 1);
            break;

        case MOTOR_REV:
            motor_set_led(_p1_19, 1);
            command_motor_reverse();
            break;

        default:
            reverse_flag = false;
            set_pwm_value(motor_1, STOP);
    }
}

void command_servo(CAR_CONTROL_t *drive_servo){
    float percent = get_servo_angle(drive_servo);
    set_pwm_value(servo_2, percent);
}

void forward_speed_control(CAR_CONTROL_t *drive_forward, double *fw_pwm_val) {
    float speed = get_speed();
//    float _error = speed - drive_forward->MOTOR_kph;
//
//    if(_error > 0)
//        *fw_pwm_val -= _error/12;
//    else if(_error < 0)
//        *fw_pwm_val -= _error/22;
//    else{}
//
//    if(*fw_pwm_val < 15.5)
//        *fw_pwm_val = 15.5;
//    else if(*fw_pwm_val > 16.3)
//        *fw_pwm_val = 16.3;

    float _error = calculate(drive_forward->MOTOR_kph, speed, &dc);
    if(_error > 0)
        *fw_pwm_val += _error*2;
    else if(_error < 0)
        *fw_pwm_val += _error*1.5;
    else{}
//    *fw_pwm_val += _error;
    if(*fw_pwm_val < 15.60)
        *fw_pwm_val = 15.60;
     if(*fw_pwm_val > 16.3)
        *fw_pwm_val = 16.3;
    set_pwm_value(motor_1, *fw_pwm_val);
//    u0_dbg_printf("SPEED: %f", drive_forward->MOTOR_kph);
//    u0_dbg_printf("PWM: %f", *fw_pwm_val);
}

void command_motor_reverse(void)
{
    if (false == reverse_flag) {

        if (5 == reverse_count)
            reverse_flag = true;
        if (6 == reverse_count)
            reverse_count = 0;

        if (0 == reverse_count % 3)
            set_pwm_value(motor_1, STOP);
        else if (1 == reverse_count % 3)
            set_pwm_value(motor_1, STOP);
        else if (2 == reverse_count % 3)
            set_pwm_value(motor_1, REVERSE);
        ++reverse_count;
    }
    else {
        set_pwm_value(motor_1, REVERSE);
        reverse_count = 0;
    }
}

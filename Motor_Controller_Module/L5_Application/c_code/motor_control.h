/*
 * motor_control.h
 *
 *  Created on: Apr 9, 2019
 *      Author: Jay
 */

#ifndef MOTOR_CONTROL_H_
#define MOTOR_CONTROL_H_

#include "motor_can_rx.h"
#include "c_io.h"
#include "c_pwm.h"
#include "speed_calculator.h"
#include "generated_can.h"

#ifdef __cplusplus
extern "C" {
#endif

void command_motor(CAR_CONTROL_t *drive_motor, double *cmd_pwm_val);
void command_servo(CAR_CONTROL_t *drive_motor);
void forward_speed_control(CAR_CONTROL_t *drive_forward, double *fw_pwm_val);
void command_motor_reverse(void);


#ifdef __cplusplus
}
#endif

#endif /* MOTOR_CONTROL_H_ */

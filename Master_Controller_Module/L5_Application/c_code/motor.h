/*
 * motor.h
 *
 *  Created on: Apr 26, 2019
 *      Author: Aakash
 */

#ifndef MOTOR_H_
#define MOTOR_H_

#ifdef __cplusplus
extern "C" {
#endif

void drive_motor_fwd_slow(void);
void drive_motor_fwd_med(void);
void drive_motor_fwd_fast(void);
void drive_motor_rev_slow(void);

void drive_motor_stop(void);

void master_steer_full_left(void);
void master_steer_full_right(void);
void master_steer_slight_left(void);
void master_steer_slight_right(void);
void master_dont_steer(void);

void service_motor_hbt_msg(void);


#ifdef __cplusplus
}
#endif


#endif /* MOTOR_H_ */

/*
 * motor.c
 *
 *  Created on: Apr 26, 2019
 *      Author: Aakash
 */
#include "../../_can_dbc/generated_can.h"
#include "c_code/defines.h"
#include "c_code/motor.h"
#include "c_code/externs.h"

inline void drive_motor_fwd_slow(void)
{
    car_control.MOTOR_DRIVE_cmd = MOTOR_FORWARD;
    car_control.MOTOR_kph = MOTOR_SLOW_KPH;
}

inline void drive_motor_fwd_med(void)
{
    car_control.MOTOR_DRIVE_cmd = MOTOR_FORWARD;
    car_control.MOTOR_kph = MOTOR_MED_KPH;
}

inline void drive_motor_fwd_fast(void)
{
    car_control.MOTOR_DRIVE_cmd = MOTOR_FORWARD;
    car_control.MOTOR_kph = MOTOR_FAST_KPH;
}

inline void drive_motor_rev_slow(void)
{
    car_control.MOTOR_DRIVE_cmd = MOTOR_REV;
    car_control.MOTOR_kph = MOTOR_SLOW_KPH;
}

inline void drive_motor_stop(void)
{
    car_control.MOTOR_DRIVE_cmd = MOTOR_STOP;
    car_control.MOTOR_kph = MOTOR_STOP_KPH;
}

inline void master_steer_full_left(void)
{
    car_control.MOTOR_STEER_cmd = MOTOR_STEER_FULL_LEFT;
}

inline void master_steer_full_right(void)
{
    car_control.MOTOR_STEER_cmd = MOTOR_STEER_FULL_RIGHT;
}

inline void master_steer_slight_left(void)
{
    car_control.MOTOR_STEER_cmd = MOTOR_STEER_SLIGHT_LEFT;
}

inline void master_steer_slight_right(void)
{
    car_control.MOTOR_STEER_cmd = MOTOR_STEER_SLIGHT_RIGHT;
}

inline void master_dont_steer(void)
{
    car_control.MOTOR_STEER_cmd = MOTOR_DONT_STEER;
}

void service_motor_hbt_msg(void)
{
    dbc_decode_MOTOR_HEARTBEAT(&motor_hbt, can_msg.data.bytes, &can_msg_hdr);
    motor_hbt_sync = motor_hbt.MOTOR_hbt;
}

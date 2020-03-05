/*
 * can_lcd.h
 *
 *  Created on: Apr 23, 2019
 *      Author: Jay
 */

#ifndef CAN_RECEIVE_H_
#define CAN_RECEIVE_H_

#include "can.h"
#include "c_io.h"
#include "speed_calculator.h"
#include "_can_dbc/generated_can.h"


#ifdef __cplusplus
extern "C" {
#endif

extern CAR_CONTROL_t drive;

int led_digit_front_obs;
char led_digit_left_obs;
char led_digit_right_obs;
char led_digit_back_obs;

float gps_cur_latitude;
float gps_cur_longitude;
float gps_dest_latitude;
float gps_dest_longitude;
float gps_bearing;
float gps_heading;
float gps_distance;
float motor_speed;
float motor_turning_angle;

uint8_t master_status, sensor_status, gps_status, bridge_status;

bool receive_can_msg(void);

#ifdef __cplusplus
}
#endif

#endif /* CAN_RECEIVE_H_ */

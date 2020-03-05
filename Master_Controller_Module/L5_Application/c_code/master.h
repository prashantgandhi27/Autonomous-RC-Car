/*
 * master.h
 *
 *  Created on: Apr 4, 2019
 *      Author: Aakash
 */

#ifndef MASTER_H_
#define MASTER_H_

#ifdef __cplusplus
extern "C" {
#endif

#include "stdbool.h"
#include "stdint.h"
#include "../../_can_dbc/generated_can.h"

void master_controller_init(void);

bool start_obstacle_avoidance(void);

void master_service_can_msgs_at_100Hz(void);

bool master_CAN_turn_on_bus_if_bus_off(void);

void master_send_command_to_motor_module(void);

bool sys_start_stop_cmd(void);

void master_mia_handler(void);

bool hbt_sync_from_all_node(void);

bool transmit_heartbeat_on_can(void);

void master_send_debug_msg(void);

uint8_t get_lidar_zone_information(uint8_t lidar_range);

uint16_t get_ultrasonic_zone_information(uint16_t distance_in_cm);

void display_hbt_sync_from_all_controllers(void);

void navigate_car_to_destination(void);

void set_drive_cmd(MOTOR_DRIVE_cmd_E drive_cmd);

MOTOR_DRIVE_cmd_E get_drive_cmd(void);

void set_drive_speed(float speed);

float get_drive_speed(void);

void set_steer_cmd(float steer_angle);

float get_steer_cmd(void);

void output_hbt_on_LED(void);

void set_bridge_node_data(BRIDGE_NODE_t node);

void set_sensor_node_data(SENSOR_NODE_t node);

void set_gps_data(GPS_LOCATION_t node);

void set_compass_data(COMPASS_t node);

void set_bridge_hbt_data(BRIDGE_HEARTBEAT_t node);

void set_sensor_hbt_data(SENSOR_HEARTBEAT_t node);

void set_motor_hbt_data(MOTOR_HEARTBEAT_t node);

void set_gps_hbt_data(GPS_HEARTBEAT_t node);

#ifdef __cplusplus
}
#endif

#endif /* MASTER_H_ */

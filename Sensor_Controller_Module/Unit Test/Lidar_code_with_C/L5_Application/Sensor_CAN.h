/*
 * Sensor_CAN.h
 *
 *  Created on: Apr 25, 2019
 *      Author: User
 */

#ifndef SENSOR_CAN_H_
#define SENSOR_CAN_H_
#ifdef __cplusplus
extern "C" {
#endif

#include "RPLidar.h"
#include <stdbool.h>
#include <stdint.h>

bool sensor_can_init(void);
bool sensor_send_data(void);
bool sensor_CAN_turn_on_bus_if_bus_off(void);
bool transmit_heartbeat_on_can(void);
bool send_sensor_debug_msg(void);

#ifdef __cplusplus
}
#endif
#endif /* SENSOR_CAN_H_ */

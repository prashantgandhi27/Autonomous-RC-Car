/*
 * Lidar_funcs.h
 *
 *  Created on: Apr 25, 2019
 *      Author: User
 */

#ifndef LIDAR_FUNCS_H_
#define LIDAR_FUNCS_H_

#ifdef __cplusplus
extern "C" {
#endif

#include <stdbool.h>
#include "RPLidar.h"
#include "Sensor_CAN.h"

bool Lidar_init(void);
void Lidar_get_data(void);
void Lidar_parse_data(void);
void Lidar_send_data(void);

#ifdef __cplusplus
}
#endif
#endif /* LIDAR_FUNCS_H_ */

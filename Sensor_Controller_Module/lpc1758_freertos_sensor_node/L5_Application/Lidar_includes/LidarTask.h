/*
 * LidarTask.h
 *
 *  Created on: Apr 5, 2019
 *      Author: halak
 */

#ifndef LIDARTASK_H_
#define LIDARTASK_H_

#include "RPLidar.h"
#include "lpc_pwm.hpp"
#include "printf_lib.h"
#include "string.h"
#include "queue.h"
#include "lpc_sys.h"
#include "scheduler_task.hpp"

class Lidar
{
public:
bool Lidar_init();

void Lidar_get_data();

void Lidar_parse_data();

void Lidar_send_data_CAN();
};

#endif /* LIDARTASK_H_ */

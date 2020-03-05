/*
 * LidarTask.cpp
 *
 *  Created on: Apr 26, 2019
 *      Author: User
 */

#include "LidarTask.h"
#include "Lidar_funcs.h"
#include "Sensor_CAN.h"

LidarTask::LidarTask(uint8_t priority):scheduler_task("LidarTask", 2000, priority)
{}

bool LidarTask::init()
{
    Lidar_init();
    return true;
}

bool LidarTask::run(void *p)
{
    Lidar_get_data();
    Lidar_parse_data();
    Lidar_send_data();
    return true;
}

/*
 * LidarTask.cpp
 *
 *  Created on: Apr 5, 2019
 *      Author: halak
 */


#include "Lidar_includes/LidarTask.h"
#include "Lidar_includes/Sensor_CAN_Interfacing.h"
#include "c_code/c_io.h"
#include "c_code/c_pwm.h"
#include "Lidar_includes/RPLidar.h"

static RPLidar lidar;
static RPLidarRotation singleRotation;
static rplidar_response_measurement_node_t raw_response_node;
static RPLidarMeasurement refined_response_buff[360];
static RPLidarMeasurement refined_response;

bool Lidar::Lidar_init()
{
    lidar.uart2.init(lidar.RPLIDAR_SERIAL_BAUDRATE,11520,11520);
    int Timeout = 2000;
    rplidar_response_device_health_t get_info;
    int current_time = sys_get_uptime_ms();
    while(!IS_OK(lidar.getHealth(get_info, lidar.RPLIDAR_DEFAULT_TIMEOUT)) && Timeout < (int)(sys_get_uptime_ms() - current_time))
    {
       lidar.reset();
       vTaskDelay(200);
    }
    uint32_t result = lidar.startScan(0,lidar.RPLIDAR_DEFAULT_TIMEOUT);
    if(!IS_OK(result))
    {
        return false;
    }
    return true;
}

void Lidar::Lidar_get_data()
{
    static int count = 0;
    count++;
    memset(refined_response_buff, 0, sizeof(refined_response_buff));
    for(int i=0;i<360;i++)
    {
        if(IS_OK(lidar.waitPoint(&raw_response_node, lidar.RPLIDAR_DEFAULT_TIMEOUT, &refined_response)))
            refined_response_buff[i] = refined_response;
    }

}

//Changes
void Lidar::Lidar_parse_data()
{
    int length = (sizeof(refined_response_buff))/(sizeof(refined_response_buff[0]));
    int *p = (int *)&singleRotation;
    for (int i = 0; i < 7; i++)
    {
        *p = 7;
        p++;
    }
    lidar.divideAngle(refined_response_buff,length, &singleRotation);
}

void Lidar::Lidar_send_data_CAN()
{
    sensor_send_data(&singleRotation);
}

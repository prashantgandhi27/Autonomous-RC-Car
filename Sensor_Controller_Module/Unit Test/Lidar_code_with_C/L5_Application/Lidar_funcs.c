/*
 * Lidar_funcs.c
 *
 *  Created on: Apr 25, 2019
 *      Author: User
 */

#include "Lidar_funcs.h"
#include <stdbool.h>
#include <stdio.h>
#include <string.h>

static RPLidarRotation singleRotation;
static rplidar_response_measurement_node_t raw_response_node;
static RPLidarMeasurement refined_response_buff[360];
static RPLidarMeasurement refined_response;

bool Lidar_init(void)
{
    rplidar_response_device_health_t get_healthinfo;
    Uart_init_for_sensor();
    getHealth(&get_healthinfo, 500);
    //u0_dbg_printf("health %d\n",data);
    startScan(500);
   // u0_dbg_printf("startscan %d\n",result);
    return 1;
}

void Lidar_get_data(void)
{
   // memset(refined_response_buff, 0, sizeof(refined_response_buff));
    for(int i=0;i<360;i++)
    {
      if(IS_OK(waitPoint(&raw_response_node, 500, &refined_response)))
      refined_response_buff[i] = refined_response;
    //  u0_dbg_printf("%d\n",refined_response_buff[i]);
    }
}

void Lidar_parse_data()
{
    int length = (sizeof(refined_response_buff))/(sizeof(refined_response_buff[0]));
       int *p = (int *)&singleRotation;
       for (int i = 0; i < 12; i++)
       {
           *p = 13;
           p++;
       }
       divideAngle(refined_response_buff,length, &singleRotation);
}

void Lidar_send_data(void)
{
   sensor_send_data();
}

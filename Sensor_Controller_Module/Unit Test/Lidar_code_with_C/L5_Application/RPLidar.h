/*
 * RPLidar.h
 *
 *  Created on: Apr 24, 2019
 *      Author: User
 */

#ifndef RPLIDAR_H_
#define RPLIDAR_H_

#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>

#include "rplidar_cmd.h"
#include "c_code/c_uart2.h"

typedef struct
{
    float distance;
    float angle;
    uint8_t quality;
    bool  startBit;
}RPLidarMeasurement;

typedef struct
{
/*    int sector_front;
    int sector_front_left;
    int sector_front_right;
    int sector_rear;*/

    int s0;
    int s1;
    int s2;
    int s3;
    int s4;
    int s5;
    int s6;
    int s7;
    int s8;
    int s9;
    int s10;
    int s11;
}RPLidarRotation;

uint32_t getHealth(rplidar_response_device_health_t *healthinfo, uint32_t timeout);
uint32_t getDeviceInfo(rplidar_response_device_info_t *info, uint32_t timeout);
uint32_t stop(void);
uint32_t reset(void);
uint32_t startScan(uint32_t timeout);
uint32_t waitPoint(rplidar_response_measurement_node_t *node, uint32_t timeout, RPLidarMeasurement *measurement);
int divideDistance(float distance);
void divideAngle(RPLidarMeasurement *angle_value, int length,RPLidarRotation *rot);
uint32_t _sendCommand(uint8_t cmd, const void * payload, size_t payloadsize);
uint32_t _waitResponseHeader(rplidar_ans_header_t * header, uint32_t timeout);
void Uart_init_for_sensor(void);



#endif /* RPLIDAR_H_ */

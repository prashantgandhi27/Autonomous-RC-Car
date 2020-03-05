/*
 * RPLidar.c

 *
 *  Created on: Apr 24, 2019
 *      Author: User
 */

#include <stdbool.h>
#include <string.h>
//#include "lpc_sys.h"
//#include "utilities.h"
#include "RPLidar.h"
#include "generated_can.h"
#include "c_code/c_uart2.h"

#define distance_range 250.0
SENSOR_NODE_t sensor_cmd;
static int previous_track[12] = {13};

void Uart_init_for_sensor()
{
    uart2_instance();
    uart2_init(115200,50,50);
}

uint32_t _sendCommand(uint8_t cmd, const void * payload, size_t payloadsize)
{
    rplidar_cmd_packet_t pkt_header;
    rplidar_cmd_packet_t * header = &pkt_header;

    if (payloadsize && payload) {
        cmd |= RPLIDAR_CMDFLAG_HAS_PAYLOAD;
    }

    header->syncByte = RPLIDAR_CMD_SYNC_BYTE;
    header->cmd_flag = cmd;
    uart2_put_char(header->syncByte, 0);
    uart2_put_char(header->cmd_flag, 0);
    return RESULT_OK;
}
uint32_t _waitResponseHeader(rplidar_ans_header_t * header, uint32_t timeout)
{
    uint8_t  recvPos = 0;
    uint32_t currentTs = 100; //sys_get_uptime_ms()
    uint32_t remainingtime;
    uint8_t *headerbuf = (uint8_t*)header;
    while ((remainingtime=100 - currentTs) <= timeout) { //sys_get_uptime_ms

        char received_data;
        uart2_get_char(&received_data,0);
        int currentbyte = received_data;

        if (currentbyte<0) continue;

        switch (recvPos) {
            case 0:
                if (currentbyte != RPLIDAR_ANS_SYNC_BYTE1) {
                    continue;
                }
                break;
            case 1:
                if (currentbyte != RPLIDAR_ANS_SYNC_BYTE2) {
                    recvPos = 0;
                    continue;
                }
                break;
        }
        headerbuf[recvPos++] = currentbyte;
        if (recvPos == sizeof(rplidar_ans_header_t)) {
            return RESULT_OK;
        }


    }

    return RESULT_OPERATION_TIMEOUT;
}

uint32_t startScan(uint32_t timeout)
{
    uint32_t ans;

    stop(); //force the previous operation to stop
    ans = _sendCommand(RPLIDAR_CMD_SCAN, NULL, 0);
    if (IS_FAIL(ans)) return ans;

    // waiting for confirmation
    rplidar_ans_header_t response_header;
    if (IS_FAIL(ans = _waitResponseHeader(&response_header, timeout))) {
        return ans;
    }

    // verify whether we got a correct header
    if (response_header.type != RPLIDAR_ANS_TYPE_MEASUREMENT) {
        return RESULT_INVALID_DATA;
    }

    if (response_header.size < sizeof(rplidar_response_measurement_node_t)) {
        return RESULT_INVALID_DATA;
    }
    return RESULT_OK;
}

uint32_t getHealth(rplidar_response_device_health_t *healthinfo, uint32_t timeout)
{
    uart2_instance();
    uart2_init(115200,50,50);
    uint32_t currentTs = 100;//sys_get_uptime_ms();
        uint32_t remainingtime;
        uint8_t *infobuf = (uint8_t *)&healthinfo;
        uint8_t recvPos = 0;

        rplidar_ans_header_t response_header;
        uint32_t  ans;
        if (IS_FAIL(ans = _sendCommand(RPLIDAR_CMD_GET_DEVICE_HEALTH, NULL, 0))) {
            return ans;
        }

        if (IS_FAIL(ans = _waitResponseHeader(&response_header, timeout))) {
            return ans;
        }

        // verify whether we got a correct header
        if (response_header.type != RPLIDAR_ANS_TYPE_DEVHEALTH) {
            return RESULT_INVALID_DATA;
        }

        if ((response_header.size) < sizeof(rplidar_response_device_health_t)) {
            return RESULT_INVALID_DATA;
        }

        while ((remainingtime=100 - currentTs) <= timeout) { //sys_get_uptime_ms
            char received_data;
            uart2_get_char(&received_data,0);
            int currentbyte = received_data;
            if (currentbyte < 0) continue;
            infobuf[recvPos++] = currentbyte;

            if (recvPos == sizeof(rplidar_response_device_health_t)) {
                return RESULT_OK;
            }
        }
        return RESULT_OPERATION_TIMEOUT;
}

uint32_t stop()
{
    uint32_t ans = _sendCommand(RPLIDAR_CMD_STOP,NULL,0);
    return ans;
}

uint32_t waitPoint(rplidar_response_measurement_node_t *node, uint32_t timeout, RPLidarMeasurement *measurement)
{
    uint32_t currentTs = 100; //sys_get_uptime_ms();
    uint32_t remainingtime;
    uint8_t *nodebuf = (uint8_t*)node;
    uint8_t recvPos = 0;
    while ((remainingtime=100 - currentTs) <= timeout) { //sys_get_uptime_ms
        char recieved_char;
        int currentbyte = 0;
        if(uart2_get_char(&recieved_char,0)){
            currentbyte = recieved_char;
        }
        if (currentbyte<0) continue;

        switch (recvPos) {
            case 0: // expect the sync bit and its reverse in this byte          {
            {
                uint8_t tmp = (currentbyte>>1);
                if ( (tmp ^ currentbyte) & 0x1 ) {
                    // pass
                } else {
                    continue;
                }

            }
            break;
            case 1: // expect the highest bit to be 1
            {
                if (currentbyte & RPLIDAR_RESP_MEASUREMENT_CHECKBIT) {
                    // pass
                } else {
                    recvPos = 0;
                    continue;
                }
            }
            break;
        }
        nodebuf[recvPos++] = currentbyte;

        if (recvPos == sizeof(rplidar_response_measurement_node_t)) {
            measurement->distance = node->distance_q2/(float)4.0;
            measurement->angle = (node->angle_q6_checkbit >> RPLIDAR_RESP_MEASUREMENT_ANGLE_SHIFT)/(float)64.0;
            measurement->quality = (node->sync_quality>>RPLIDAR_RESP_MEASUREMENT_QUALITY_SHIFT);
            measurement->startBit = (node->sync_quality & RPLIDAR_RESP_MEASUREMENT_SYNCBIT);
            return RESULT_OK;
        }
    }
    return RESULT_OPERATION_TIMEOUT;
}

int divideDistance(float distance)
{
    int track;
    if ((distance > 280) && (distance <= 280+distance_range)) {
        track = 1;
    }
    else if ((distance > 280+distance_range) && (distance <= 280+(distance_range * 2))) {
        track = 2;
    }
    else if ((distance > 280+(distance_range * 2)) && (distance <= 280+(distance_range * 3))) {
        track = 3;
    }
    else if ((distance > 280+(distance_range * 3)) && (distance <= 280+(distance_range * 4))) {
        track = 4;
    }
    else if ((distance > 280+(distance_range * 4)) && (distance <= 280+(distance_range * 5))) {
        track = 5;
    }
    else if ((distance > 280+(distance_range * 5)) && (distance <= 280+(distance_range * 6))) {
        track = 6;
    }
    else if ((distance > 280+(distance_range * 6)) && (distance <= 280+(distance_range * 7))) {
        track = 7;
    }
    else if ((distance > 280+(distance_range * 7)) && (distance <= 280+(distance_range * 8))) {
        track = 8;
    }
    else if ((distance > 280+(distance_range * 8)) && (distance <= 280+(distance_range * 9))) {
        track = 9;
    }
    else if ((distance > distance_range * 9) && (distance <= distance_range * 10)) {
        track = 10;
    }
    else if ((distance > distance_range * 10) && (distance <= distance_range * 11)) {
        track = 11;
    }
    else if ((distance > distance_range * 11) && (distance <= distance_range * 12)) {
        track = 12;
    }
    else {
        track = 13;
    }
    return track;
}

void divideAngle(RPLidarMeasurement *measurement_buff, int length, RPLidarRotation *rot)
{

    for(int j=0;j<12;j++)
        {
                previous_track[j] = 13;
        }

    for(int i=0; i<length; i++)
    {
      int track = 13;
      if(measurement_buff[i].quality!=0){
          if(measurement_buff[i].angle >= 0.0 && measurement_buff[i].angle <= DEGREES10) //0-10
                 {
                     track = divideDistance(measurement_buff[i].distance);
                     rot->s0 = previous_track[0];
                     if (track < previous_track[0]) rot->s0 = track;
                     previous_track[0] = track;
                     sensor_cmd.LIDAR_Obstacle_FRONT = rot->s0;
                 }
                 else if(measurement_buff[i].angle > DEGREES10 && measurement_buff[i].angle <= DEGREES20)
                   { //10-20
                      track = divideDistance(measurement_buff[i].distance);
                      rot->s1 = previous_track[1];
                       if (track < previous_track[1]) rot->s1 = track;
                       previous_track[1] = track;
                       sensor_cmd.LIDAR_Obstacle_RIGHT  = rot->s1;
                   }
                   else if(measurement_buff[i].angle > DEGREES20 && measurement_buff[i].angle <= DEGREES45)
                   {//20-45
                       track = divideDistance(measurement_buff[i].distance);
                       rot->s2 = previous_track[2];
                       if (track < previous_track[2]) rot->s2 = track;
                       previous_track[2] = track;
                       sensor_cmd.LIDAR_Obstacle_RIGHT  = rot->s2;
                   }
                   else if(measurement_buff[i].angle > DEGREES120 && measurement_buff[i].angle <= DEGREES150)
                   {
                       track = divideDistance(measurement_buff[i].distance);
                       rot->s4 = previous_track[4];
                       if (track < previous_track[4]) rot->s4 = track;
                       previous_track[4] = track;
                       sensor_cmd.LIDAR_Obstacle_BACK  = rot->s4;
                   }
                   else if(measurement_buff[i].angle > DEGREES150 && measurement_buff[i].angle <= DEGREES180)
                   {
                       track = divideDistance(measurement_buff[i].distance);
                       rot->s5 = previous_track[5];
                       if (track < previous_track[5]) rot->s5 = track;
                       previous_track[5] = track;
                       sensor_cmd.LIDAR_Obstacle_BACK  = rot->s5;
                   }
                   else if(measurement_buff[i].angle > DEGREES180 && measurement_buff[i].angle <= DEGREES210)
                   {
                       track = divideDistance(measurement_buff[i].distance);
                       rot->s6 = previous_track[6];
                       if (track < previous_track[6]) rot->s6 = track;
                       previous_track[6] = track;
                       sensor_cmd.LIDAR_Obstacle_BACK  = rot->s6;
                   }
                   else if(measurement_buff[i].angle > DEGREES240 && measurement_buff[i].angle <= DEGREES270)
                   {
                       track = divideDistance(measurement_buff[i].distance);
                       rot->s7 = previous_track[7];
                       if (track < previous_track[7]) rot->s7 = track;
                       previous_track[7] = track;
                       sensor_cmd.LIDAR_Obstacle_BACK  = rot->s7;
                   }
                   else if(measurement_buff[i].angle > DEGREES315 && measurement_buff[i].angle <= DEGREES335)
                   {
                       track = divideDistance(measurement_buff[i].distance);
                       rot->s9 = previous_track[9];
                       if (track < previous_track[9]) rot->s9 = track;
                       previous_track[9] = track;
                       sensor_cmd.LIDAR_Obstacle_LEFT = rot->s9;
                   }
                   else if(measurement_buff[i].angle > DEGREES335 && measurement_buff[i].angle <= DEGREES345)
                   {
                       track = divideDistance(measurement_buff[i].distance);
                       rot->s10 = previous_track[10];
                       if (track < previous_track[10]) rot->s10 = track;
                       previous_track[10] = track;
                       sensor_cmd.LIDAR_Obstacle_LEFT = rot->s10;
                   }
                   else if(measurement_buff[i].angle > DEGREES345 && measurement_buff[i].angle <= DEGREES355)
                   {
                       track = divideDistance(measurement_buff[i].distance);
                       rot->s11 = previous_track[11];
                       if (track < previous_track[11]) rot->s11 = track;
                       previous_track[11] = track;
                       sensor_cmd.LIDAR_Obstacle_FRONT  = rot->s11;
                   }
               }
    }
}

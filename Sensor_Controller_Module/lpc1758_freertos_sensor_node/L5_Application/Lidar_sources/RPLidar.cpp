/*
 * RoboPeak RPLIDAR Driver for Arduino
 * RoboPeak.com
 *
 * Copyright (c) 2014, RoboPeak
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without modification,
 * are permitted provided that the following conditions are met:
 *
 * 1. Redistributions of source code must retain the above copyright notice,
 *    this list of conditions and the following disclaimer.
 *
 * 2. Redistributions in binary form must reproduce the above copyright notice,
 *    this list of conditions and the following disclaimer in the documentation
 *    and/or other materials provided with the distribution.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND ANY
 * EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES
 * OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT
 * SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL,
 * SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT
 * OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION)
 * HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR
 * TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE,
 * EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 *
 */
#include <string.h>
#include <stdio.h>
#include <stddef.h>
#include "lpc_sys.h"
#include "utilities.h"
#include "Lidar_includes/RPLidar.h"
#include "_can_dbc/generated_can.h"
#include "c_code/c_gpio.h"

//#define distance_range 250.0
SENSOR_NODE_t sensor_cmd;
static int previous_track[8] = {8};

// ask the RPLIDAR for its health info
uint32_t RPLidar::getHealth(rplidar_response_device_health_t & healthinfo, uint32_t timeout)
{
    uint32_t currentTs = sys_get_uptime_ms();
    uint32_t remainingtime;
    uint8_t *infobuf = (uint8_t *)&healthinfo;
    uint8_t recvPos = 0;

    rplidar_ans_header_t response_header;
    u_result  ans;
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

    while ((remainingtime=sys_get_uptime_ms() - currentTs) <= timeout) {
        char received_data;
        uart2.getChar(&received_data,portMAX_DELAY);
        int currentbyte = received_data;
        if (currentbyte < 0) continue;
        infobuf[recvPos++] = currentbyte;

        if (recvPos == sizeof(rplidar_response_device_health_t)) {
            return RESULT_OK;
        }
    }
    return RESULT_OPERATION_TIMEOUT;
}

// ask the RPLIDAR for its device info like the serial number
uint32_t RPLidar::getDeviceInfo(rplidar_response_device_info_t & info, uint32_t timeout )
{
    uint8_t  recvPos = 0;
    uint32_t currentTs = sys_get_uptime_ms();
    uint32_t remainingtime;
    uint8_t *infobuf = (uint8_t*)&info;
    rplidar_ans_header_t response_header;
    u_result  ans;

    {
        if (IS_FAIL(ans = _sendCommand(RPLIDAR_CMD_GET_DEVICE_INFO,NULL,0))) {
            return ans;
        }

        if (IS_FAIL(ans = _waitResponseHeader(&response_header, timeout))) {
            return ans;
        }

        // verify whether we got a correct header
        if (response_header.type != RPLIDAR_ANS_TYPE_DEVINFO) {
            return RESULT_INVALID_DATA;
        }

        if (response_header.size < sizeof(rplidar_response_device_info_t)) {
            return RESULT_INVALID_DATA;
        }

        while ((remainingtime=sys_get_uptime_ms() - currentTs) <= timeout) {

            char received_data;
            uart2.getChar(&received_data,portMAX_DELAY);
            int currentbyte = received_data;
            if (currentbyte<0) continue;
            infobuf[recvPos++] = currentbyte;

            if (recvPos == sizeof(rplidar_response_device_info_t)) {
                return RESULT_OK;
            }
        }
    }

    return RESULT_OPERATION_TIMEOUT;
}

// stop the measurement operation
uint32_t RPLidar::stop()
{
    uint32_t ans = _sendCommand(RPLIDAR_CMD_STOP,NULL,0);
    return ans;
}

uint32_t RPLidar::reset()
{
    uint32_t ans = _sendCommand(RPLIDAR_CMD_RESET,NULL,0);
    return ans;
}


// start the measurement operation
uint32_t RPLidar::startScan(bool force, uint32_t timeout)
{
    uint32_t ans;

    stop(); //force the previous operation to stop

    ans = _sendCommand(force?RPLIDAR_CMD_FORCE_SCAN:RPLIDAR_CMD_SCAN, NULL, 0);
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



uint32_t RPLidar::_sendCommand(uint8_t cmd, const void * payload, size_t payloadsize)
{
    rplidar_cmd_packet_t pkt_header;
    rplidar_cmd_packet_t * header = &pkt_header;

    if (payloadsize && payload) {
        cmd |= RPLIDAR_CMDFLAG_HAS_PAYLOAD;
    }

    header->syncByte = RPLIDAR_CMD_SYNC_BYTE;
    header->cmd_flag = cmd;
    uart2.putChar(header->syncByte, portMAX_DELAY);
    uart2.putChar(header->cmd_flag, portMAX_DELAY);
    return RESULT_OK;
}

uint32_t RPLidar::_waitResponseHeader(rplidar_ans_header_t * header, uint32_t timeout)
{
    uint8_t  recvPos = 0;
    uint32_t currentTs = sys_get_uptime_ms();
    uint32_t remainingtime;
    uint8_t *headerbuf = (uint8_t*)header;
    while ((remainingtime=sys_get_uptime_ms() - currentTs) <= timeout) {

        char received_data;
        uart2.getChar(&received_data,portMAX_DELAY);
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

uint32_t RPLidar::waitPoint(rplidar_response_measurement_node_t *node, uint32_t timeout, RPLidarMeasurement *measurement)
{
    uint32_t currentTs = sys_get_uptime_ms();
    uint32_t remainingtime;
    uint8_t *nodebuf = (uint8_t*)node;
    uint8_t recvPos = 0;
    while ((remainingtime=sys_get_uptime_ms() - currentTs) <= timeout) {
        char recieved_char;
        int currentbyte = 0;
        if(uart2.getChar(&recieved_char,portMAX_DELAY)){
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

int RPLidar::divideDistance(float distance)
{
    int track = 7;
    if ((distance > 0) && (distance <= 250)) {
        track = 7;
    }
    else if ((distance > 250) && (distance <= 500)) {
        track = 1;
    }
    else if ((distance > 500) && (distance <= 750)) {
        track = 2;
    }
    else if ((distance > 750) && (distance <= 1000)) {
        track = 3;
    }
    else if ((distance > 1000) && (distance <= 1250)) {
        track = 4;
    }
    else if ((distance > 1250) && (distance <= 1500)) {
        track = 5;
    }
    else if ((distance > 1500) && (distance <= 1750)) {
        track = 6;
    }
    else {
        track = 7;
    }
    return track;
}
//int RPLidar::divideDistanceback(float distance)
//{
//    int track = 7;
//        if ((distance > 0) && (distance <= 80)) {
//            track = 0;
//        }
//        else if ((distance > 80) && (distance <= 330)) {
//            track = 1;
//        }
//        else if ((distance > 330) && (distance <= 580)) {
//            track = 2;
//        }
//        else if ((distance > 580) && (distance <= 830)) {
//            track = 3;
//        }
//        else if ((distance > 1080) && (distance <= 1340)) {
//            track = 4;
//        }
//        else {
//            track = 7;
//        }
//        return track;
//}
//void RPLidar::divideAngle(RPLidarMeasurement *measurement_buff, int length, RPLidarRotation *rot)
//{
//
//    for(int j=0;j<8;j++)
//    {
//        previous_track[j] = 8;
//    }
//
//    for(int i=0; i<length; i++)
//    {
//        int track = 8;
//        if(measurement_buff[i].quality!=0){
//            if(measurement_buff[i].angle >= DEGREE_0 && measurement_buff[i].angle < DEGREE_10) //0-10
//            {
//                track = divideDistancefront(measurement_buff[i].distance);
//                if (track < previous_track[0])
//                {
//                    rot->s0 = track;
//                    previous_track[0] = track;
//                }
//                sensor_cmd.LIDAR_Obstacle_FRONT = rot->s0;
//            }
//            else if(measurement_buff[i].angle >= DEGREE_10 && measurement_buff[i].angle < DEGREE_20) //10-20
//            {
//                track = divideDistancefront(measurement_buff[i].distance);
//                if (track < previous_track[1])
//                {
//                    rot->s0 = track;
//                    previous_track[0] = track;
//                }
//                sensor_cmd.LIDAR_Obstacle_FRONT = rot->s0;
//            }
//            else if(measurement_buff[i].angle >= DEGREE_20 && measurement_buff[i].angle < DEGREE_30) //20-30
//            {
//                track = divideDistancefront(measurement_buff[i].distance);
//                rot->s1 = previous_track[1];
//                if (track < previous_track[1])
//                {
//                    rot->s1 = track;
//
//                }
//                previous_track[1] = track;
//                sensor_cmd.LIDAR_Obstacle_RIGHT = rot->s1;
//            }
//            else if(measurement_buff[i].angle >= DEGREE_30 && measurement_buff[i].angle < DEGREE_40) //30-40
//            {
//                track = divideDistancefront(measurement_buff[i].distance);
//                rot->s1 = previous_track[1];
//                if (track < previous_track[1])
//                {
//                    rot->s1 = track;
//
//                }
//                previous_track[1] = track;
//                sensor_cmd.LIDAR_Obstacle_RIGHT = rot->s1;
//            }
//            else if(measurement_buff[i].angle >= DEGREE_40 && measurement_buff[i].angle < DEGREE_50) //40-50
//            {
//                track = divideDistancefront(measurement_buff[i].distance);
//                rot->s1 = previous_track[1];
//                if (track < previous_track[1])
//                {
//                    rot->s1 = track;
//
//                }
//                previous_track[1] = track;
//                sensor_cmd.LIDAR_Obstacle_RIGHT = rot->s1;
//            }
////            else if(measurement_buff[i].angle >= DEGREE_50 && measurement_buff[i].angle < DEGREE_60) //50-60
////            {
////
////            }
////            else if(measurement_buff[i].angle >= DEGREE_60 && measurement_buff[i].angle < DEGREE_70) //60-70
////            {
////
////            }
////            else if(measurement_buff[i].angle >= DEGREE_70 && measurement_buff[i].angle < DEGREE_80) //70-80
////            {
////
////            }
////            else if(measurement_buff[i].angle >= DEGREE_80 && measurement_buff[i].angle < DEGREE_90) //80-90
////            {
////
////            }
////            else if(measurement_buff[i].angle >= DEGREE_90 && measurement_buff[i].angle < DEGREE_100) //90-100
////            {
////
////            }
////            else if(measurement_buff[i].angle >= DEGREE_100 && measurement_buff[i].angle < DEGREE_110) //100-110
////            {
////
////            }
////            else if(measurement_buff[i].angle >= DEGREE_110 && measurement_buff[i].angle < DEGREE_120) //110-120
////            {
////
////            }
////            else if(measurement_buff[i].angle >= DEGREE_120 && measurement_buff[i].angle < DEGREE_130) //120-130
////            {
////
////            }
////            else if(measurement_buff[i].angle >= DEGREE_130 && measurement_buff[i].angle < DEGREE_140) //130-140
////            {
////
////            }
//            else if(measurement_buff[i].angle >= DEGREE_140 && measurement_buff[i].angle < DEGREE_150) //140-150
//            {
//                track = divideDistanceback(measurement_buff[i].distance);
//                if (track < previous_track[2])
//                {
//                    rot->s2 = track;
//                    previous_track[2] = track;
//                }
//                sensor_cmd.LIDAR_Obstacle_BACK_RIGHT = rot->s2;
//            }
//            else if(measurement_buff[i].angle >= DEGREE_150 && measurement_buff[i].angle < DEGREE_160) //150-160
//            {
//                track = divideDistanceback(measurement_buff[i].distance);
//                if (track < previous_track[2])
//                {
//                    rot->s2 = track;
//                    previous_track[2] = track;
//                }
//                sensor_cmd.LIDAR_Obstacle_BACK_RIGHT = rot->s2;
//            }
//            else if(measurement_buff[i].angle >= DEGREE_160 && measurement_buff[i].angle < DEGREE_170) //160-170
//            {
//                track = divideDistanceback(measurement_buff[i].distance);
//                if (track < previous_track[2])
//                {
//                    rot->s2 = track;
//                    previous_track[2] = track;
//                }
//                sensor_cmd.LIDAR_Obstacle_BACK_RIGHT = rot->s2;
//            }
//            else if(measurement_buff[i].angle >= DEGREE_170 && measurement_buff[i].angle < DEGREE_180) //170-180
//            {
//                track = divideDistanceback(measurement_buff[i].distance);
//                if (track < previous_track[3])
//                {
//                    rot->s3 = track;
//                    previous_track[3] = track;
//                }
//                sensor_cmd.LIDAR_Obstacle_BACK = rot->s3;
//            }
//            else if(measurement_buff[i].angle >= DEGREE_180 && measurement_buff[i].angle < DEGREE_190) //180-190
//            {
//                track = divideDistanceback(measurement_buff[i].distance);
//                if (track < previous_track[3])
//                {
//                    rot->s3 = track;
//                    previous_track[3] = track;
//                }
//                sensor_cmd.LIDAR_Obstacle_BACK = rot->s3;
//            }
//            else if(measurement_buff[i].angle >= DEGREE_190 && measurement_buff[i].angle < DEGREE_200) //190-200
//            {
//                track = divideDistanceback(measurement_buff[i].distance);
//                if (track < previous_track[4])
//                {
//                    rot->s4 = track;
//                    previous_track[4] = track;
//                }
//                sensor_cmd.LIDAR_Obstacle_BACK_LEFT = rot->s4;
//            }
//            else if(measurement_buff[i].angle >= DEGREE_200 && measurement_buff[i].angle < DEGREE_210) //200-210
//            {
//                track = divideDistanceback(measurement_buff[i].distance);
//                if (track < previous_track[4])
//                {
//                    rot->s4 = track;
//                    previous_track[4] = track;
//                }
//                sensor_cmd.LIDAR_Obstacle_BACK_LEFT = rot->s4;
//            }
//            else if(measurement_buff[i].angle >= DEGREE_210 && measurement_buff[i].angle < DEGREE_220) //210-220
//            {
//                track = divideDistanceback(measurement_buff[i].distance);
//                if (track < previous_track[4])
//                {
//                    rot->s4 = track;
//                    previous_track[4] = track;
//                }
//                sensor_cmd.LIDAR_Obstacle_BACK_LEFT = rot->s4;
//            }
//            else if(measurement_buff[i].angle >= DEGREE_220 && measurement_buff[i].angle < DEGREE_230) //220-230
//            {
//
//            }
//            else if(measurement_buff[i].angle >= DEGREE_230 && measurement_buff[i].angle < DEGREE_240) //230-240
//            {
//
//            }
//            else if(measurement_buff[i].angle >= DEGREE_240 && measurement_buff[i].angle < DEGREE_250) //240-250
//            {
//
//            }
//            else if(measurement_buff[i].angle >= DEGREE_250 && measurement_buff[i].angle < DEGREE_260) //250-260
//            {
//
//            }
//            else if(measurement_buff[i].angle >= DEGREE_260 && measurement_buff[i].angle < DEGREE_270) //260-270
//            {
//
//            }
//            else if(measurement_buff[i].angle >= DEGREE_270 && measurement_buff[i].angle < DEGREE_280) //270-280
//            {
//
//            }
//            else if(measurement_buff[i].angle >= DEGREE_280 && measurement_buff[i].angle < DEGREE_290) //280-290
//            {
//
//            }
//            else if(measurement_buff[i].angle >= DEGREE_290 && measurement_buff[i].angle < DEGREE_300) //290-300
//            {
//
//            }
//            else if(measurement_buff[i].angle >= DEGREE_300 && measurement_buff[i].angle < DEGREE_310) //300-310-290
//            {
//
//            }
//            else if(measurement_buff[i].angle >= DEGREE_310 && measurement_buff[i].angle < DEGREE_320) //310-320
//            {
//                track = divideDistancefront(measurement_buff[i].distance);
//                rot->s5 = previous_track[5];
//                if (track < previous_track[5])
//                {
//                    rot->s5 = track;
//
//                }
//                previous_track[5] = track;
//                sensor_cmd.LIDAR_Obstacle_LEFT = rot->s5;
//            }
//            else if(measurement_buff[i].angle >= DEGREE_320 && measurement_buff[i].angle < DEGREE_330) //320-330
//            {
//                track = divideDistancefront(measurement_buff[i].distance);
//                rot->s5 = previous_track[5];
//                if (track < previous_track[5])
//                {
//                    rot->s5 = track;
//
//                }
//                previous_track[5] = track;
//                sensor_cmd.LIDAR_Obstacle_LEFT = rot->s5;
//            }
//            else if(measurement_buff[i].angle >= DEGREE_330 && measurement_buff[i].angle < DEGREE_340) //330-340
//            {
//                track = divideDistancefront(measurement_buff[i].distance);
//                rot->s5 = previous_track[5];
//                if (track < previous_track[5])
//                {
//                    rot->s5 = track;
//
//                }
//                previous_track[5] = track;
//                sensor_cmd.LIDAR_Obstacle_LEFT = rot->s5;
//            }
//            else if(measurement_buff[i].angle >= DEGREE_340 && measurement_buff[i].angle < DEGREE_350) //340-350
//            {
//                track = divideDistancefront(measurement_buff[i].distance);
//                if (track < previous_track[0])
//                {
//                    rot->s0 = track;
//                    previous_track[5] = track;
//                }
//                sensor_cmd.LIDAR_Obstacle_FRONT = rot->s0;
//            }
//            else if(measurement_buff[i].angle >= DEGREE_350 && measurement_buff[i].angle < DEGREE_360) //350-360
//            {
//                track = divideDistancefront(measurement_buff[i].distance);
//                if (track < previous_track[0])
//                {
//                    rot->s0 = track;
//                    previous_track[0] = track;
//                }
//                sensor_cmd.LIDAR_Obstacle_FRONT = rot->s0;
//            }
//        }
//        set_LEDS();
//    }
//
//}
void RPLidar::divideAngle(RPLidarMeasurement *measurement_buff, int length, RPLidarRotation *rot)
{

#if 1
    for(int j=0;j<8;j++)
    {
        previous_track[j] = 7;
    }

    for(int i=0; i<length; i++)
    {
        int track = 8;
        if(measurement_buff[i].quality!=0){
            if(measurement_buff[i].angle >= DEGREE_0 && measurement_buff[i].angle < DEGREE_10) //0-10
            {
                track = divideDistance(measurement_buff[i].distance);
                if (track < previous_track[0])
                {
                    previous_track[0] = track;
                    rot->s0 = track;
                }

                sensor_cmd.LIDAR_Obstacle_FRONT = rot->s0;
            }
            else if(measurement_buff[i].angle >= DEGREE_10 && measurement_buff[i].angle < DEGREE_20) //10-20
            {
                track = divideDistance(measurement_buff[i].distance);
                if (track < previous_track[1])
                {
                    previous_track[0] = track;
                }
                rot->s0 = track;
                sensor_cmd.LIDAR_Obstacle_FRONT = rot->s0;
            }
            else if(measurement_buff[i].angle >= DEGREE_20 && measurement_buff[i].angle < DEGREE_30) //20-30
            {
                track = divideDistance(measurement_buff[i].distance);
                if (track < previous_track[1])
                {
                    previous_track[1] = track;
                    rot->s1 = track;
                }

                sensor_cmd.LIDAR_Obstacle_RIGHT = rot->s1;
            }
            else if(measurement_buff[i].angle >= DEGREE_30 && measurement_buff[i].angle < DEGREE_40) //30-40
            {
                track = divideDistance(measurement_buff[i].distance);
                if (track < previous_track[1])
                {
                    previous_track[1] = track;
                    rot->s1 = track;
                }

                sensor_cmd.LIDAR_Obstacle_RIGHT = rot->s1;
            }
            else if(measurement_buff[i].angle >= DEGREE_40 && measurement_buff[i].angle < DEGREE_50) //40-50
            {
                track = divideDistance(measurement_buff[i].distance);
                if (track < previous_track[1])
                {
                    previous_track[1] = track;
                    rot->s1 = track;
                }

                sensor_cmd.LIDAR_Obstacle_RIGHT = rot->s1;
            }

            else if(measurement_buff[i].angle >= DEGREE_140 && measurement_buff[i].angle < DEGREE_150) //140-150
            {
                track = divideDistance(measurement_buff[i].distance);
                if (track < previous_track[2])
                {
                    previous_track[2] = track;
                }
                rot->s2 = track;
                sensor_cmd.LIDAR_Obstacle_BACK_RIGHT = rot->s2;
            }
            else if(measurement_buff[i].angle >= DEGREE_150 && measurement_buff[i].angle < DEGREE_160) //150-160
            {
                track = divideDistance(measurement_buff[i].distance);
                if (track < previous_track[2])
                {
                    previous_track[2] = track;
                }
                rot->s2 = track;
                sensor_cmd.LIDAR_Obstacle_BACK_RIGHT = rot->s2;
            }
            else if(measurement_buff[i].angle >= DEGREE_160 && measurement_buff[i].angle < DEGREE_170) //160-170
            {
                track = divideDistance(measurement_buff[i].distance);
                if (track < previous_track[2])
                {
                    previous_track[2] = track;
                }
                rot->s2 = track;
                sensor_cmd.LIDAR_Obstacle_BACK_RIGHT = rot->s2;
            }
            else if(measurement_buff[i].angle >= DEGREE_170 && measurement_buff[i].angle < DEGREE_180) //170-180
            {
                track = divideDistance(measurement_buff[i].distance);
                if (track < previous_track[3])
                {
                    previous_track[3] = track;
                }
                rot->s3 = track;
                sensor_cmd.LIDAR_Obstacle_BACK = rot->s3;
            }
            else if(measurement_buff[i].angle >= DEGREE_180 && measurement_buff[i].angle < DEGREE_190) //180-190
            {
                track = divideDistance(measurement_buff[i].distance);
                if (track < previous_track[3])
                {
                    previous_track[3] = track;
                }
                rot->s3 = track;
                sensor_cmd.LIDAR_Obstacle_BACK = rot->s3;
            }
            else if(measurement_buff[i].angle >= DEGREE_190 && measurement_buff[i].angle < DEGREE_200) //190-200
            {
                track = divideDistance(measurement_buff[i].distance);
                if (track < previous_track[4])
                {
                    previous_track[4] = track;
                }
                rot->s4 = track;
                sensor_cmd.LIDAR_Obstacle_BACK_LEFT = rot->s4;
            }
            else if(measurement_buff[i].angle >= DEGREE_200 && measurement_buff[i].angle < DEGREE_210) //200-210
            {
                track = divideDistance(measurement_buff[i].distance);
                if (track < previous_track[4])
                {
                    previous_track[4] = track;
                }
                rot->s4 = track;
                sensor_cmd.LIDAR_Obstacle_BACK_LEFT = rot->s4;
            }
            else if(measurement_buff[i].angle >= DEGREE_210 && measurement_buff[i].angle < DEGREE_220) //210-220
            {
                track = divideDistance(measurement_buff[i].distance);
                if (track < previous_track[4])
                {
                    previous_track[4] = track;
                }
                rot->s4 = track;
                sensor_cmd.LIDAR_Obstacle_BACK_LEFT = rot->s4;
            }

            else if(measurement_buff[i].angle >= DEGREE_310 && measurement_buff[i].angle < DEGREE_320) //310-320
            {
                track = divideDistance(measurement_buff[i].distance);
                if (track < previous_track[5])
                {
                    rot->s5 = track;
                    previous_track[5] = track;
                }

                sensor_cmd.LIDAR_Obstacle_LEFT = rot->s5;
            }
            else if(measurement_buff[i].angle >= DEGREE_320 && measurement_buff[i].angle < DEGREE_330) //320-330
            {
                track = divideDistance(measurement_buff[i].distance);
                if (track < previous_track[5])
                {
                    rot->s5 = track;
                    previous_track[5] = track;
                }

                sensor_cmd.LIDAR_Obstacle_LEFT = rot->s5;
            }
            else if(measurement_buff[i].angle >= DEGREE_330 && measurement_buff[i].angle < DEGREE_340) //330-340
            {
                track = divideDistance(measurement_buff[i].distance);
                if (track < previous_track[5])
                {

                    previous_track[5] = track;
                    rot->s5 = track;
                }

                sensor_cmd.LIDAR_Obstacle_LEFT = rot->s5;
            }
            else if(measurement_buff[i].angle >= DEGREE_340 && measurement_buff[i].angle < DEGREE_350) //340-350
            {
                track = divideDistance(measurement_buff[i].distance);
                if (track < previous_track[0])
                {
                    rot->s0 = track;
                    previous_track[5] = track;
                }

                sensor_cmd.LIDAR_Obstacle_FRONT = rot->s0;
            }
            else if(measurement_buff[i].angle >= DEGREE_350 && measurement_buff[i].angle < DEGREE_360) //350-360
            {
                track = divideDistance(measurement_buff[i].distance);
                if (track < previous_track[0])
                {
                    rot->s0 = track;
                    previous_track[0] = track;
                }
                sensor_cmd.LIDAR_Obstacle_FRONT = rot->s0;
            }
        }
        set_LEDS();
    }
#endif
}


void RPLidar::set_LEDS()
{
    if(sensor_cmd.LIDAR_Obstacle_RIGHT>0 && sensor_cmd.LIDAR_Obstacle_RIGHT<5)
    {
        set_Ext_LED(P1_20,1);
    }
    else
    {
        set_Ext_LED(P1_20,0);
    }

    if(sensor_cmd.LIDAR_Obstacle_BACK>0 &&sensor_cmd.LIDAR_Obstacle_BACK<5)
    {
        set_Ext_LED(P0_29,1);
    }
    else
    {
        set_Ext_LED(P0_29,0);
    }

    if(sensor_cmd.LIDAR_Obstacle_LEFT>0 && sensor_cmd.LIDAR_Obstacle_LEFT<5)
    {
        set_Ext_LED(P0_30,1);

    }
    else
    {
        set_Ext_LED(P0_30,0);

    }
}

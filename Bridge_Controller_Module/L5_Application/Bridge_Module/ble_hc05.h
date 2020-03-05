/*
 * BLE_HC05.h
 *
 *  Created on: Mar 31, 2019
 *      Author: Vidushi
 */

#ifndef BLE_HC05_H_
#define BLE_HC05_H_
#ifdef __cplusplus
extern "C" {
#endif
#include <stdint.h>
#include "c_io.h"
#include "../can_code/CAN_Communication.h"
#include "c_wrapper.h"
#include <string.h>

#define FIRMWARE_VERSION    "1.00"


void BLE_init(void);
void BLE_tx(void);
void BLE_rx(void);
char* find_list_element(char* list_str, int index, char separator);

enum dataFromApp {
    STOP_CMD = 0,
    START_CMD = 1,
    CONTINUE_CMD = 2
};

/**
 *
 */
typedef struct {
    char getdata[120];
    char MotorDriveCmd[10];
    char MotorSteerCmd[10];
    char RPM[10];
    char obstacleFront[12];
    char obstacleRight[12];
    char obstacleLeft[10];
    char obstacleBack[10];
    char latitude[15];
    char longitude[15];
    char bearing[15];
    char heading[15];
    char distance[15];
} __attribute__((__packed__)) ble_msg_t;


#ifdef __cplusplus
}
#endif
#endif /* BLE_HC05_H_ */

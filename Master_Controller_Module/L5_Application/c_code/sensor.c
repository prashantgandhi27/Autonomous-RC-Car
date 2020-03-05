/*
 * sensor.c
 *
 *  Created on: Apr 17, 2019
 *      Author: Aakash
 */
#include "sensor.h"
#include "c_io.h"
#include "externs.h"

void service_sensor_msg(void)
{
    dbc_decode_SENSOR_NODE(&sensor_data, can_msg.data.bytes, &can_msg_hdr);
    setLED(2,false);
}

void service_sensor_hbt_msg(void)
{
    dbc_decode_SENSOR_HEARTBEAT(&sensor_hbt, can_msg.data.bytes, &can_msg_hdr);
    sensor_hbt_sync = sensor_hbt.SENSOR_hbt;
}

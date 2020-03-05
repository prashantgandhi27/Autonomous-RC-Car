/*
 * compass.c
 *
 *  Created on: Apr 17, 2019
 *      Author: Aakash
 */
#include "compass.h"
#include "c_io.h"
#include "externs.h"

void service_compass_msg(void)
{
    dbc_decode_COMPASS(&compass_data, can_msg.data.bytes, &can_msg_hdr);
    setLED(4, false);
}

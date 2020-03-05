/*
 * bridge.c
 *
 *  Created on: Apr 17, 2019
 *      Author: Aakash
 */
#include "bridge.h"
#include "c_io.h"
#include "externs.h"

void service_bridge_msg(void)
{
    dbc_decode_BRIDGE_NODE(&bridge_data, can_msg.data.bytes, &can_msg_hdr);
    start_cmd = bridge_data.BRIDGE_START_cmd;
    setLED(1,false);
}

void service_bridge_hbt_msg(void)
{
    dbc_decode_BRIDGE_HEARTBEAT(&bridge_hbt, can_msg.data.bytes, &can_msg_hdr);
    bridge_hbt_sync = bridge_hbt.BRIDGE_hbt;
}


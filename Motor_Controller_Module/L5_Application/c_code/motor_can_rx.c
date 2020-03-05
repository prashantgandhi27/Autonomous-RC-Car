/*
 * c_can.c
 *
 *  Created on: Mar 3, 2019
 *      Author: Aakash
 */
#include "motor_can_rx.h"
#include "printf_lib.h"

const can_t can = can1;
const uint32_t baudRate = 100;
const uint16_t rxQueueSize = 10;
const uint16_t txQueueSize = 10;
const uint32_t timeout_ms = 0;

bool motor_can_init(void) {
    bool flag = false;
    if (CAN_init(can, baudRate, rxQueueSize, txQueueSize, 0, 0)){
       CAN_bypass_filter_accept_all_msgs();
        CAN_reset_bus(can);
        flag = true;
         return flag;
    }
    else
         return flag;
}

bool motor_can_tx_heartbeat(void) {
    MOTOR_HEARTBEAT_t heartbeat_msg = { 0 };
    can_msg_t can_msg = { 0 };

    heartbeat_msg.MOTOR_hbt = 1;

    dbc_msg_hdr_t msg_hdr = dbc_encode_MOTOR_HEARTBEAT(can_msg.data.bytes, &heartbeat_msg);
    can_msg.msg_id = msg_hdr.mid;
    can_msg.frame_fields.data_len = msg_hdr.dlc;

    // Queue the CAN message to be sent out
    return (CAN_tx(can1, &can_msg, 0));
}

bool send_rpm(void) {
    MOTOR_NODE_t rpm = { 0 };
    can_msg_t can_msg = { 0 };

    rpm.MOTOR_SPEED_kph = get_speed();

    dbc_msg_hdr_t msg_hdr = dbc_encode_MOTOR_NODE(can_msg.data.bytes, &rpm);
    can_msg.msg_id = msg_hdr.mid;
    can_msg.frame_fields.data_len = msg_hdr.dlc;
    return (CAN_tx(can1, &can_msg, 0));
}

bool motor_can_reset_busoff(void) {
    if (CAN_is_bus_off(can))
    {
        CAN_bypass_filter_accept_all_msgs();
        CAN_reset_bus(can);
        return true;
    }
    return false;
}

/*
 * can_lcd.c
 *
 *  Created on: Apr 23, 2019
 *      Author: Jay
 */
#include "can_receive.h"
#include "c_pwm.h"
#include "c_gpio.h"
#include "speed_calculator.h"

#define TIMEOUT_MS 0

extern CAR_CONTROL_t drive;

const uint32_t                             BRIDGE_NODE__MIA_MS = 3000;
const BRIDGE_NODE_t                        BRIDGE_NODE__MIA_MSG = { 0 };
const uint32_t                             SENSOR_NODE__MIA_MS = 3000;
const SENSOR_NODE_t                        SENSOR_NODE__MIA_MSG = {0};
const uint32_t                             GPS_LOCATION__MIA_MS = 3000;
const GPS_LOCATION_t                       GPS_LOCATION__MIA_MSG = {0};
const uint32_t                             COMPASS__MIA_MS = 3000;
const COMPASS_t                            COMPASS__MIA_MSG = {0};
const uint32_t                             MASTER_HEARTBEAT__MIA_MS = 3000;
const MASTER_HEARTBEAT_t                   MASTER_HEARTBEAT__MIA_MSG = {0};
const uint32_t                             CAR_CONTROL__MIA_MS = 3000;
const CAR_CONTROL_t                        CAR_CONTROL__MIA_MSG = { MOTOR_STOP, -90.00, 0, { 0 } };
static BRIDGE_NODE_t bridge_msg = {0};
static BRIDGE_CHECKPOINTS_t bridge_checkpoint_msg = {0};
static SENSOR_NODE_t sensor_msg = {0};
static GPS_LOCATION_t gps_msg = {0};
static COMPASS_t compass_msg = {0};
static MASTER_HEARTBEAT_t master_hrtbt = {0};

bool receive_can_msg(void){
    can_msg_t can_motor_msg;
    dbc_msg_hdr_t can_msg_hdr;
     bool motor_rx_flag = false;
    while (CAN_rx(can1, &can_motor_msg, TIMEOUT_MS))
    {
        can_msg_hdr.dlc = can_motor_msg.frame_fields.data_len;
        can_msg_hdr.mid = can_motor_msg.msg_id;
        uint32_t mid = can_motor_msg.msg_id;

        switch (mid)
        {
            case 103: // Bridge
                dbc_decode_BRIDGE_NODE(&bridge_msg, can_motor_msg.data.bytes, &can_msg_hdr);
                bridge_status = 1;
                break;
            case 104: //Master Node
                dbc_decode_CAR_CONTROL(&drive, can_motor_msg.data.bytes, &can_msg_hdr);
                motor_speed = drive.MOTOR_kph;
                motor_turning_angle = drive.MOTOR_STEER_cmd;
                master_status = 1;
                setLED(1, 0);
                break;
            case 105: // Sensor Node
                dbc_decode_SENSOR_NODE(&sensor_msg, can_motor_msg.data.bytes, &can_msg_hdr);
                led_digit_front_obs =  sensor_msg.SENSOR_FRONT_cm;
                led_digit_left_obs =  sensor_msg.LIDAR_Obstacle_LEFT;
                led_digit_back_obs =  sensor_msg.LIDAR_Obstacle_BACK;
                led_digit_right_obs =  sensor_msg.LIDAR_Obstacle_RIGHT;
                sensor_status = 1;
                break;
            case 107: // GPS
                dbc_decode_BRIDGE_CHECKPOINTS(&bridge_checkpoint_msg, can_motor_msg.data.bytes, &can_msg_hdr);
                gps_dest_latitude = bridge_checkpoint_msg.CHECKPOINT_LAT_deg;
                gps_dest_longitude = bridge_checkpoint_msg.CHECKPOINT_LONG_deg;
                break;
            case 108: // GPS
                dbc_decode_GPS_LOCATION(&gps_msg, can_motor_msg.data.bytes, &can_msg_hdr);
                gps_cur_latitude = gps_msg.CURRENT_LAT_deg;
                gps_cur_longitude = gps_msg.CURRENT_LONG_deg;
                gps_status = 1;
                break;
            case 109: // Compass
                dbc_decode_COMPASS(&compass_msg, can_motor_msg.data.bytes, &can_msg_hdr);
                gps_heading = compass_msg.CMP_HEADING_deg;
                gps_bearing = compass_msg.CMP_BEARING_deg;
                break;
            case 110: // Master heartbeat
                dbc_decode_MASTER_HEARTBEAT(&master_hrtbt, can_motor_msg.data.bytes, &can_msg_hdr);
                motor_set_led(_p1_20, 1);
                break;
        }
    }

    if(dbc_handle_mia_SENSOR_NODE(&sensor_msg, 100))
    {
        sensor_status = 0;
    }
    if (dbc_handle_mia_CAR_CONTROL(&drive, 100)) {
        master_status = 0;
        drive.MOTOR_DRIVE_cmd = 2;
        drive.MOTOR_STEER_cmd = 0.0;
        setLED(1, 1);
    }
    if(dbc_handle_mia_GPS_LOCATION(&gps_msg, 100))
    {
        gps_status = 0;
    }

    if(dbc_handle_mia_BRIDGE_NODE(&bridge_msg, 100))
    {
        bridge_status = 0;
    }
    if(dbc_handle_mia_MASTER_HEARTBEAT(&master_hrtbt, 30)) {
        motor_set_led(_p1_20, 0);
        master_status = 0;
    }
    return motor_rx_flag;
}


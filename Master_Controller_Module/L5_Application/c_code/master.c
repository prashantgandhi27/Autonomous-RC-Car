/*
 * master.cpp
 *
 *  Created on: Apr 4, 2019
 *      Author: Aakash
 */
#include "can.h"
#include "c_code/defines.h"
#include "c_code/master.h"
#include "c_code/c_io.h"
#include <stdio.h>

static BRIDGE_NODE_t bridge_data = {0, {0}};
static SENSOR_NODE_t sensor_data = {0};
static GPS_LOCATION_t gps_data = {0, 0, {0}};
static COMPASS_t compass_data = {0};
static CAR_CONTROL_t car_control = { 0, 0.0, 0.0 };
static SENSOR_HEARTBEAT_t sensor_hbt = {0};
static MOTOR_HEARTBEAT_t motor_hbt = {0};
static GPS_HEARTBEAT_t gps_hbt = {0};
static BRIDGE_HEARTBEAT_t bridge_hbt = {0};

 can_msg_t can_msg;
 dbc_msg_hdr_t can_msg_hdr;

const uint32_t                             BRIDGE_NODE__MIA_MS = 3000;
const BRIDGE_NODE_t                        BRIDGE_NODE__MIA_MSG = { 0, {0} };
const uint32_t                             SENSOR_NODE__MIA_MS = 3000;
const SENSOR_NODE_t                        SENSOR_NODE__MIA_MSG = { 0 };
const uint32_t                             GPS_LOCATION__MIA_MS = 3000;
const GPS_LOCATION_t                       GPS_LOCATION__MIA_MSG = { 37.3686485, -121.9153289, {0} };
const uint32_t                             COMPASS__MIA_MS = 3000;
const COMPASS_t                            COMPASS__MIA_MSG = { 37.3686485, -121.9153289, 0.0, {0} };
const uint32_t                             SENSOR_HEARTBEAT__MIA_MS = 3000;
const SENSOR_HEARTBEAT_t                   SENSOR_HEARTBEAT__MIA_MSG = {0, {0}};
const uint32_t                             MOTOR_HEARTBEAT__MIA_MS = 3000;
const MOTOR_HEARTBEAT_t                    MOTOR_HEARTBEAT__MIA_MSG = {0, {0}};
const uint32_t                             GPS_HEARTBEAT__MIA_MS = 3000;
const GPS_HEARTBEAT_t                      GPS_HEARTBEAT__MIA_MSG = {0, {0}};
const uint32_t                             BRIDGE_HEARTBEAT__MIA_MS = 3000;
const BRIDGE_HEARTBEAT_t                   BRIDGE_HEARTBEAT__MIA_MSG = {0, {0}};

static bool start_cmd = false;
static bool sensor_hbt_sync = false;
static bool motor_hbt_sync = false;
static bool gps_hbt_sync = false;
static bool bridge_hbt_sync = false;

static bool can_init_flag = false;
static bool bus_off_flag = false;

bool start_free_run_flag = false;
bool free_run_motor_flag = false;
bool free_steer_flag = false;
static bool check_rear_obstacle = false;
static bool is_hbt_sync_flag = false;

void master_controller_init(void)
{
    do {
        can_init_flag = CAN_init(CAN_BUS, CAN_BAUD_RATE, CAN_RX_QUEUE_SIZE, CAN_TX_QUEUE_SIZE, 0, 0);
    } while(false == can_init_flag);

    CAN_bypass_filter_accept_all_msgs();
    CAN_reset_bus(CAN_BUS);
}

bool start_obstacle_avoidance()
{
    bool is_obstacle = false;
    // determine zone for lidar and ultrasonic
    uint16_t front_sensor = get_ultrasonic_zone_information(sensor_data.SENSOR_FRONT_cm);
    uint8_t obs_front = get_lidar_zone_information(sensor_data.LIDAR_Obstacle_FRONT);
    uint8_t obs_left = get_lidar_zone_information(sensor_data.LIDAR_Obstacle_LEFT);
    uint8_t obs_right = get_lidar_zone_information(sensor_data.LIDAR_Obstacle_RIGHT);
    uint8_t obs_back = get_lidar_zone_information(sensor_data.LIDAR_Obstacle_BACK);

    car_control.MOTOR_DRIVE_cmd = MOTOR_FORWARD;
    car_control.MOTOR_kph = MOTOR_SLOW_KPH;

#if 1
    if (true == check_rear_obstacle)
    {
        printf("Reverse\n");
        if (obs_back == NEAR)
        {
            car_control.MOTOR_DRIVE_cmd = MOTOR_STOP;
            car_control.MOTOR_kph = MOTOR_STOP_KPH;
        }
        else if (obs_back == MID)
        {
            car_control.MOTOR_DRIVE_cmd = MOTOR_REV;
        }
        else
        {
            car_control.MOTOR_DRIVE_cmd = MOTOR_REV;
        }
    }

    if ((front_sensor == VERY_NEAR) && (false == check_rear_obstacle))
    {
        car_control.MOTOR_DRIVE_cmd = MOTOR_STOP;
        car_control.MOTOR_kph = MOTOR_STOP_KPH;
        car_control.MOTOR_STEER_cmd = STRAIGHT;
        check_rear_obstacle = true;
        return true;
    }

    // Avoid obstacle
    // if zone is near
    if ((obs_left == NEAR) || (obs_front == NEAR) || (obs_right == NEAR))
    {
        car_control.MOTOR_kph = MOTOR_VERY_SLOW_KPH;
        if((obs_left == NEAR) && (obs_front == NEAR) && (obs_right == NEAR) && (false == check_rear_obstacle))
        {
            car_control.MOTOR_DRIVE_cmd = MOTOR_STOP;
            car_control.MOTOR_kph = MOTOR_STOP_KPH;
            car_control.MOTOR_STEER_cmd = STRAIGHT;
            check_rear_obstacle = true;
        }
        else if((obs_left != NEAR) && (obs_front == NEAR) && (obs_right != NEAR) && (false == check_rear_obstacle))
        {
            car_control.MOTOR_DRIVE_cmd = MOTOR_STOP;
            car_control.MOTOR_kph = MOTOR_STOP_KPH;
            car_control.MOTOR_STEER_cmd = STRAIGHT;
            check_rear_obstacle = true;
        }
        else if((obs_left == NEAR ) && (obs_front != NEAR) && (obs_right == NEAR) && (false == check_rear_obstacle))
        {
            car_control.MOTOR_DRIVE_cmd = MOTOR_STOP;
            car_control.MOTOR_kph = MOTOR_STOP_KPH;
            car_control.MOTOR_STEER_cmd = STRAIGHT;
            check_rear_obstacle = true;
        }
        else if((obs_left != NEAR) && (obs_front != NEAR) && (obs_right == NEAR))
        {
            car_control.MOTOR_STEER_cmd = FULL_LEFT;
            check_rear_obstacle = false;
        }
        else if((obs_left == NEAR) && (obs_front != NEAR) && (obs_right != NEAR))
        {
            car_control.MOTOR_STEER_cmd = FULL_RIGHT;
            check_rear_obstacle = false;
        }
        else if((obs_left != NEAR) && (obs_front == NEAR) && (obs_right == NEAR))
        {
            car_control.MOTOR_STEER_cmd = FULL_LEFT;
            check_rear_obstacle = false;
        }
        else if((obs_left == NEAR) && (obs_front == NEAR) && (obs_right != NEAR))
        {
            car_control.MOTOR_STEER_cmd = FULL_RIGHT;
            check_rear_obstacle = false;
        }
        is_obstacle = true;
    }
    else if ((obs_left == MID) || (obs_front == MID) || (obs_right == MID))
    {
        check_rear_obstacle = false;
        car_control.MOTOR_kph = MOTOR_SLOW_KPH;
        if((obs_left != MID) && (front_sensor != MID) &&(obs_right == MID))
        {
            car_control.MOTOR_STEER_cmd = SLIGHT_LEFT;
        }
        else if((obs_left != MID) && (obs_front == MID) &&(obs_right != MID))
        {
            if(obs_left < obs_right)
            {
                car_control.MOTOR_STEER_cmd = SLIGHT_RIGHT;
            }
            else if(obs_left > obs_right)
            {
                car_control.MOTOR_STEER_cmd = SLIGHT_LEFT;
            }
            else
            {
                car_control.MOTOR_STEER_cmd = STRAIGHT;
            }
        }
        else if((obs_left != MID) && (obs_front == MID) &&(obs_right == MID))
        {
            car_control.MOTOR_STEER_cmd = SLIGHT_LEFT;
        }
        else if((obs_left == MID) && (obs_front != MID) &&(obs_right != MID))
        {
            car_control.MOTOR_STEER_cmd = SLIGHT_RIGHT;
        }
        else if((obs_left == MID) && (obs_front != MID) &&(obs_right == MID))
        {
            car_control.MOTOR_STEER_cmd = STRAIGHT;
        }
        else if((obs_left == MID) && (obs_front == MID) &&(obs_right != MID))
        {
            car_control.MOTOR_STEER_cmd = SLIGHT_RIGHT;
        }
        else if((obs_left == MID) && (obs_front == MID) &&(obs_right == MID))
        {
            car_control.MOTOR_STEER_cmd = STRAIGHT;
        }
        is_obstacle = true;
    }
    else
    {
        check_rear_obstacle = false;
        car_control.MOTOR_STEER_cmd = STRAIGHT;
        is_obstacle = false;
    }
    return is_obstacle;
#endif
}

void master_service_can_msgs_at_100Hz(void)
{
    while(CAN_rx(CAN_BUS, &can_msg, 0))
    {
        // Form the message header from the metadata of the arriving message
        can_msg_hdr.dlc = can_msg.frame_fields.data_len;
        can_msg_hdr.mid = can_msg.msg_id;
        if (can_msg_hdr.mid == BRIDGE_NODE_HDR.mid) {
            dbc_decode_BRIDGE_NODE(&bridge_data, can_msg.data.bytes, &can_msg_hdr);
            start_cmd = bridge_data.BRIDGE_START_cmd;
            setLED(1,false);
        } else if (can_msg_hdr.mid == SENSOR_NODE_HDR.mid) {
            dbc_decode_SENSOR_NODE(&sensor_data, can_msg.data.bytes, &can_msg_hdr);
            setLED(2,false);
        } else if (can_msg_hdr.mid == GPS_LOCATION_HDR.mid) {
            dbc_decode_GPS_LOCATION(&gps_data, can_msg.data.bytes, &can_msg_hdr);
            setLED(3, false);
        } else if (can_msg_hdr.mid == COMPASS_HDR.mid) {
            dbc_decode_COMPASS(&compass_data, can_msg.data.bytes, &can_msg_hdr);
            setLED(4, false);
        } else if (can_msg_hdr.mid == SENSOR_HEARTBEAT_HDR.mid) {
            dbc_decode_SENSOR_HEARTBEAT(&sensor_hbt, can_msg.data.bytes, &can_msg_hdr);
            sensor_hbt_sync = sensor_hbt.SENSOR_hbt;
        } else if (can_msg_hdr.mid == MOTOR_HEARTBEAT_HDR.mid) {
            dbc_decode_MOTOR_HEARTBEAT(&motor_hbt, can_msg.data.bytes, &can_msg_hdr);
            motor_hbt_sync = motor_hbt.MOTOR_hbt;
        } else if (can_msg_hdr.mid == GPS_HEARTBEAT_HDR.mid) {
            dbc_decode_GPS_HEARTBEAT(&gps_hbt, can_msg.data.bytes, &can_msg_hdr);
            gps_hbt_sync = gps_hbt.GPS_hbt;
        } else if (can_msg_hdr.mid == BRIDGE_HEARTBEAT_HDR.mid) {
            dbc_decode_BRIDGE_HEARTBEAT(&bridge_hbt, can_msg.data.bytes, &can_msg_hdr);
            bridge_hbt_sync = bridge_hbt.BRIDGE_hbt;
        } else {
            // Undefined message
        }
    }
    master_mia_handler();
}

void master_mia_handler(void)
{
    if (dbc_handle_mia_BRIDGE_NODE(&bridge_data, 100))
    {
        setLED(1, true);
    }
    if (dbc_handle_mia_SENSOR_NODE(&sensor_data, 100))
    {
        setLED(2, true);
    }
    if (dbc_handle_mia_GPS_LOCATION(&gps_data, 100))
    {
        setLED(3, true);
    }
    if (dbc_handle_mia_COMPASS(&compass_data, 100))
    {
        setLED(4, true);
    }
    if (dbc_handle_mia_SENSOR_HEARTBEAT(&sensor_hbt, 25))
    {
        sensor_hbt_sync = false;
    }
    if (dbc_handle_mia_MOTOR_HEARTBEAT(&motor_hbt, 25))
    {
        motor_hbt_sync = false;
    }
    if (dbc_handle_mia_GPS_HEARTBEAT(&gps_hbt, 25))
    {
        gps_hbt_sync = false;
    }
    if (dbc_handle_mia_BRIDGE_HEARTBEAT(&bridge_hbt, 25))
    {
        bridge_hbt_sync = false;
    }
}

bool hbt_sync_from_all_node(void)
{
    is_hbt_sync_flag = sensor_hbt_sync & motor_hbt_sync & gps_hbt_sync & bridge_hbt_sync;
    return is_hbt_sync_flag;
}

void master_send_command_to_motor_module(void)
{
    dbc_msg_hdr_t msg_hdr_tx = {0};
    msg_hdr_tx = dbc_encode_CAR_CONTROL(can_msg.data.bytes,&car_control);
    can_msg.msg_id = msg_hdr_tx.mid;
    can_msg.frame_fields.data_len = msg_hdr_tx.dlc;
    CAN_tx(CAN_BUS, &can_msg, 0);
}

bool master_CAN_turn_on_bus_if_bus_off(void)
{
    if (CAN_is_bus_off(CAN_BUS))
    {
        bus_off_flag = true;
        CAN_bypass_filter_accept_all_msgs();
        CAN_reset_bus(CAN_BUS);
        return true;
    }
    bus_off_flag = false;
    return false;
}

bool sys_start_stop_cmd()
{
    return start_cmd;
}

bool transmit_heartbeat_on_can(void)
{
    MASTER_HEARTBEAT_t heartbeat_msg = { 0 };
    heartbeat_msg.MASTER_hbt = HEART_BEAT_cmd;
    dbc_msg_hdr_t msg_hdr = dbc_encode_MASTER_HEARTBEAT(can_msg.data.bytes, &heartbeat_msg);
    can_msg.msg_id = msg_hdr.mid;
    can_msg.frame_fields.data_len = msg_hdr.dlc;
    // Queue the CAN message to be sent out
    return (CAN_tx(CAN_BUS, &can_msg, 0));
}

void master_send_debug_msg(void)
{
    MASTER_DEBUG_t master_dbg_msg = {0};
    master_dbg_msg.IO_DEBUG_CAN_init = can_init_flag;
    master_dbg_msg.IO_DEBUG_HBT_FROM_ALL_CONTR = hbt_sync_from_all_node();
    master_dbg_msg.IO_DEBUG_drive_mode = start_free_run_flag;
    master_dbg_msg.IO_DEBUG_bus_off = bus_off_flag;

    dbc_msg_hdr_t msg_hdr = dbc_encode_MASTER_DEBUG(can_msg.data.bytes, &master_dbg_msg);
    can_msg.msg_id = msg_hdr.mid;
    can_msg.frame_fields.data_len = msg_hdr.dlc;
    // Queue the CAN message to be sent out
    CAN_tx(CAN_BUS, &can_msg, 0);
}

uint8_t get_lidar_zone_information(uint8_t lidar_range)
{
    if(lidar_range < LIDAR_ZONE_NEAR_THRESHOLD)
    {
        return NEAR;
    }
    else if((lidar_range >= LIDAR_ZONE_NEAR_THRESHOLD) && (lidar_range < LIDAR_ZONE_MID_THRESHOLD))
    {
        return MID;
    }
    else if((lidar_range >= LIDAR_ZONE_MID_THRESHOLD) && (lidar_range < LIDAR_ZONE_FAR_THRESHOLD))
    {
        return FAR;
    }
    else
    {
        return NO_OBSTACLE;
    }
}

uint16_t get_ultrasonic_zone_information(uint16_t distance_in_cm)
{
    if (distance_in_cm < ULTRASONIC_ZONE_VERY_NEAR)
    {
        return VERY_NEAR;
    }
    if((distance_in_cm >= ULTRASONIC_ZONE_VERY_NEAR) && (distance_in_cm < ULTRASONIC_ZONE_NEAR))
    {
        return NEAR;
    }
    else if((distance_in_cm >= ULTRASONIC_ZONE_NEAR) && (distance_in_cm < ULTRASONIC_ZONE_MID))
    {
        return MID;
    }
    else if((distance_in_cm >= ULTRASONIC_ZONE_MID) && (distance_in_cm < ULTRASONIC_ZONE_FAR))
    {
        return FAR;
    }
    else
    {
        return NO_OBSTACLE;
    }
}

void display_hbt_sync_from_all_controllers(void)
{
//    dispDataSetRightDigit(is_hbt_sync_flag + 0x30);
    output_hbt_on_LED();
}

void navigate_car_to_destination(void)
{
    if ((int)compass_data.CMP_DISTANCE_meters >= 5)
    {
        car_control.MOTOR_DRIVE_cmd = MOTOR_FORWARD;
        car_control.MOTOR_kph = MOTOR_SLOW_KPH;
        double car_bearing = compass_data.CMP_BEARING_deg;
        double target_bearing = compass_data.CMP_HEADING_deg;
        double turning_angle = target_bearing - car_bearing;
        if (turning_angle > 180.0)
        {
            turning_angle -= 360.0;
        }
        else if (turning_angle < -180.0)
        {
            turning_angle += 360.0;
        }
        car_control.MOTOR_STEER_cmd = turning_angle;
    }
    else
    {
        car_control.MOTOR_DRIVE_cmd = MOTOR_STOP;
        car_control.MOTOR_kph = MOTOR_STOP_KPH;
        car_control.MOTOR_STEER_cmd = STRAIGHT;
    }
}

void set_drive_cmd(MOTOR_DRIVE_cmd_E drive_cmd)
{
    car_control.MOTOR_DRIVE_cmd = drive_cmd;
}

MOTOR_DRIVE_cmd_E get_drive_cmd(void)
{
    return car_control.MOTOR_DRIVE_cmd;
}

void set_drive_speed(float speed)
{
    car_control.MOTOR_kph = speed;
}

float get_drive_speed(void)
{
    return car_control.MOTOR_kph;
}

void set_steer_cmd(float steer_angle)
{
    car_control.MOTOR_STEER_cmd = steer_angle;
}

float get_steer_cmd(void)
{
    return car_control.MOTOR_STEER_cmd;
}

void output_hbt_on_LED(void)
{
    set_Ext_LED(BRIDGE_LED, bridge_hbt_sync);
    set_Ext_LED(SENSOR_LED, sensor_hbt_sync);
    set_Ext_LED(MOTOR_LED, motor_hbt_sync);
    set_Ext_LED(GEO_LED, gps_hbt_sync);
}

void set_bridge_node_data(BRIDGE_NODE_t node)
{
    bridge_data.BRIDGE_START_cmd = node.BRIDGE_START_cmd;
    bridge_data.mia_info = node.mia_info;
}

void set_sensor_node_data(SENSOR_NODE_t node)
{

    sensor_data.LIDAR_Obstacle_BACK = node.LIDAR_Obstacle_BACK;
    sensor_data.LIDAR_Obstacle_BACK_LEFT = node.LIDAR_Obstacle_BACK_LEFT;
    sensor_data.LIDAR_Obstacle_BACK_RIGHT = node.LIDAR_Obstacle_BACK_RIGHT;
    sensor_data.LIDAR_Obstacle_FRONT = node.LIDAR_Obstacle_FRONT;
    sensor_data.LIDAR_Obstacle_LEFT = node.LIDAR_Obstacle_LEFT;
    sensor_data.LIDAR_Obstacle_RIGHT = node.LIDAR_Obstacle_RIGHT;
    sensor_data.SENSOR_FRONT_cm = node.SENSOR_FRONT_cm;
    sensor_data.mia_info = node.mia_info;
}

void set_gps_data(GPS_LOCATION_t node)
{
    gps_data.CURRENT_LAT_deg = node.CURRENT_LAT_deg;
    gps_data.CURRENT_LONG_deg = node.CURRENT_LONG_deg;
    gps_data.mia_info = node.mia_info;
}

void set_compass_data(COMPASS_t node)
{
    compass_data.CMP_BEARING_deg = node.CMP_BEARING_deg;
    compass_data.CMP_DISTANCE_meters = node.CMP_BEARING_deg;
    compass_data.CMP_HEADING_deg = node.CMP_BEARING_deg;
    compass_data.mia_info = node.mia_info;
}

void set_bridge_hbt_data(BRIDGE_HEARTBEAT_t node)
{
    bridge_hbt.BRIDGE_hbt = node.BRIDGE_hbt;
    bridge_hbt.mia_info = node.mia_info;
}

void set_sensor_hbt_data(SENSOR_HEARTBEAT_t node)
{
    sensor_hbt.SENSOR_hbt = node.SENSOR_hbt;
    sensor_hbt.mia_info = node.mia_info;
}

void set_motor_hbt_data(MOTOR_HEARTBEAT_t node)
{
    motor_hbt.MOTOR_hbt = node.MOTOR_hbt;
    motor_hbt.mia_info = node.mia_info;
}

void set_gps_hbt_data(GPS_HEARTBEAT_t node)
{
    gps_hbt.GPS_hbt = node.GPS_hbt;
    gps_hbt.mia_info = node.mia_info;
}

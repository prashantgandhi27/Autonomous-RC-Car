/*
 * test_master.c
 *
 *  Created on: Apr 9, 2019
 *      Author: Aakash
 */
#include "unity.h"
#include "Mockc_io.h"
#include "Mockcan.h"
#include "c_code/externs.h"
#include "c_code/defines.h"
#include "c_code/master.h"
#include "c_code/structures.h"
#include "c_code/c_io.h"
#include <stdio.h>
#include "Mockmotor.h"
#include "Mockbridge.h"
#include "Mocksensor.h"
#include "Mockgps.h"
#include "Mockcompass.h"

can_t can;
void setUp(void)
{
    can = can1;
}
void tearDown(void)
{

}

void test_master_controller_init(void)
{
    //setup expectation
    CAN_bypass_filter_accept_all_msgs_Expect();
    CAN_reset_bus_Expect(CAN_BUS);

    CAN_init_ExpectAndReturn(can, 100, 15, 10, NULL, NULL, true);
    master_controller_init();
}

void test_master_CAN_turn_on_bus_if_bus_off__return_true(void)
{
    //setup expectation
    CAN_bypass_filter_accept_all_msgs_Expect();
    CAN_reset_bus_Expect(can);
    CAN_is_bus_off_ExpectAndReturn(can, true);
    TEST_ASSERT_TRUE(master_CAN_turn_on_bus_if_bus_off());
}

void test_master_CAN_turn_on_bus_if_bus_off__return_false(void)
{
    //setup expectation
    CAN_is_bus_off_ExpectAndReturn(can, false);
    TEST_ASSERT_FALSE(master_CAN_turn_on_bus_if_bus_off());
}

void test_sys_start_stop_cmd__return_false(void)
{
    start_cmd = false;
    TEST_ASSERT_FALSE(sys_start_stop_cmd());
}

void test_sys_start_stop_cmd__return_true(void)
{
    start_cmd = true;
    TEST_ASSERT_TRUE(sys_start_stop_cmd());
}

void test_transmit_heartbeat_on_can__return_true(void)
{
    MASTER_HEARTBEAT_t heartbeat_msg = { 0 };
    can_msg_t can_msg = {0};
    heartbeat_msg.MASTER_hbt = 1;
    dbc_msg_hdr_t msg_hdr = dbc_encode_MASTER_HEARTBEAT(can_msg.data.bytes, &heartbeat_msg);
    can_msg.msg_id = msg_hdr.mid;
    can_msg.frame_fields.data_len = msg_hdr.dlc;
    CAN_tx_ExpectAndReturn(can, &can_msg, 0, true);
    TEST_ASSERT_TRUE(transmit_heartbeat_on_can());
}

void test_transmit_heartbeat_on_can__return_false(void)
{
    MASTER_HEARTBEAT_t heartbeat_msg = { 0 };
    can_msg_t can_msg = {0};
    heartbeat_msg.MASTER_hbt = 1;
    dbc_msg_hdr_t msg_hdr = dbc_encode_MASTER_HEARTBEAT(can_msg.data.bytes, &heartbeat_msg);
    can_msg.msg_id = msg_hdr.mid;
    can_msg.frame_fields.data_len = msg_hdr.dlc;
    CAN_tx_ExpectAndReturn(can1, &can_msg, 0, false);
    TEST_ASSERT_FALSE(transmit_heartbeat_on_can());
}

void test_hbt_sync_from_all_node__return_true(void)
{
    sensor_hbt_sync = true;
    motor_hbt_sync = true;
    gps_hbt_sync = true;
    bridge_hbt_sync = true;
    TEST_ASSERT_TRUE(hbt_sync_from_all_node());
}

void test_start_obstacle_avoidance__no_obstacle(void)
{
    sensor_data.SENSOR_FRONT_cm         = 101;
    sensor_data.LIDAR_Obstacle_RIGHT    = 5;
    sensor_data.LIDAR_Obstacle_LEFT     = 5;
    sensor_data.LIDAR_Obstacle_BACK     = 5;

    drive_motor_fwd_med_Expect();
    master_dont_steer_Expect();
    start_obstacle_avoidance();
}

void test_start_obstacle_avoidance__only_front_obstacle_check_if(void)
{
    sensor_data.SENSOR_FRONT_cm         = 99;
    sensor_data.LIDAR_Obstacle_RIGHT    = 6;
    sensor_data.LIDAR_Obstacle_LEFT     = 5;
    sensor_data.LIDAR_Obstacle_BACK     = 5;

    drive_motor_fwd_slow_Expect();
    master_steer_full_right_Expect();
    start_obstacle_avoidance();
}

void test_start_obstacle_avoidance__only_front_obstacle_check_else_if(void)
{
    sensor_data.SENSOR_FRONT_cm         = 99;
    sensor_data.LIDAR_Obstacle_RIGHT    = 5;
    sensor_data.LIDAR_Obstacle_LEFT     = 6;
    sensor_data.LIDAR_Obstacle_BACK     = 5;

    drive_motor_fwd_slow_Expect();
    master_steer_full_left_Expect();
    start_obstacle_avoidance();
}

void test_start_obstacle_avoidance__only_front_obstacle_check_else(void)
{
    sensor_data.SENSOR_FRONT_cm         = 99;
    sensor_data.LIDAR_Obstacle_RIGHT    = 6;
    sensor_data.LIDAR_Obstacle_LEFT     = 6;
    sensor_data.LIDAR_Obstacle_BACK     = 5;

    drive_motor_fwd_slow_Expect();
    master_steer_full_left_Expect();
    start_obstacle_avoidance();
}

void test_start_obstacle_avoidance__only_left_obstacle(void)
{
    sensor_data.SENSOR_FRONT_cm         = 101;
    sensor_data.LIDAR_Obstacle_RIGHT    = 6;
    sensor_data.LIDAR_Obstacle_LEFT     = 3;
    sensor_data.LIDAR_Obstacle_BACK     = 5;

    drive_motor_fwd_slow_Expect();
    master_steer_slight_right_Expect();
    start_obstacle_avoidance();
}

void test_start_obstacle_avoidance__only_right_obstacle(void)
{
    sensor_data.SENSOR_FRONT_cm         = 101;
    sensor_data.LIDAR_Obstacle_RIGHT    = 3;
    sensor_data.LIDAR_Obstacle_LEFT     = 5;
    sensor_data.LIDAR_Obstacle_BACK     = 5;

    drive_motor_fwd_slow_Expect();
    master_steer_slight_left_Expect();
    start_obstacle_avoidance();
}

void test_start_obstacle_avoidance__front_and_left_obstacle(void)
{
    sensor_data.SENSOR_FRONT_cm         = 99;
    sensor_data.LIDAR_Obstacle_RIGHT    = 6;
    sensor_data.LIDAR_Obstacle_LEFT     = 3;
    sensor_data.LIDAR_Obstacle_BACK     = 5;

    drive_motor_fwd_slow_Expect();
    master_steer_full_right_Expect();
    start_obstacle_avoidance();
}

void test_start_obstacle_avoidance__front_and_right_obstacle(void)
{
    sensor_data.SENSOR_FRONT_cm         = 99;
    sensor_data.LIDAR_Obstacle_RIGHT    = 3;
    sensor_data.LIDAR_Obstacle_LEFT     = 6;
    sensor_data.LIDAR_Obstacle_BACK     = 5;

    drive_motor_fwd_slow_Expect();
    master_steer_full_left_Expect();
    start_obstacle_avoidance();
}

void test_start_obstacle_avoidance__left_and_right_obstacle_check_if(void)
{
    sensor_data.SENSOR_FRONT_cm         = 150;
    sensor_data.LIDAR_Obstacle_RIGHT    = 3;
    sensor_data.LIDAR_Obstacle_LEFT     = 2;
    sensor_data.LIDAR_Obstacle_BACK     = 5;

    drive_motor_fwd_slow_Expect();
    master_steer_slight_right_Expect();
    start_obstacle_avoidance();
}

void test_start_obstacle_avoidance__left_and_right_obstacle_check_else_if(void)
{
    sensor_data.SENSOR_FRONT_cm         = 100;
    sensor_data.LIDAR_Obstacle_RIGHT    = 2;
    sensor_data.LIDAR_Obstacle_LEFT     = 3;
    sensor_data.LIDAR_Obstacle_BACK     = 5;

    drive_motor_fwd_slow_Expect();
    master_steer_slight_left_Expect();
    start_obstacle_avoidance();
}

void test_start_obstacle_avoidance__left_and_right_obstacle_check_else(void)
{
    sensor_data.SENSOR_FRONT_cm         = 100;
    sensor_data.LIDAR_Obstacle_RIGHT    = 3;
    sensor_data.LIDAR_Obstacle_LEFT     = 3;
    sensor_data.LIDAR_Obstacle_BACK     = 5;

    drive_motor_fwd_slow_Expect();
    master_dont_steer_Expect();
    start_obstacle_avoidance();
}

void test_start_obstacle_avoidance__left_and_right_and_front_obstacle_check_else(void)
{
    sensor_data.SENSOR_FRONT_cm         = 99;
    sensor_data.LIDAR_Obstacle_RIGHT    = 3;
    sensor_data.LIDAR_Obstacle_LEFT     = 3;
    sensor_data.LIDAR_Obstacle_BACK     = 5;

    drive_motor_stop_Expect();
    master_dont_steer_Expect();
    start_obstacle_avoidance();
}

void test_start_obstacle_avoidance__check_rear_obstacle_if(void)
{
    sensor_data.SENSOR_FRONT_cm         = 99;
    sensor_data.LIDAR_Obstacle_RIGHT    = 3;
    sensor_data.LIDAR_Obstacle_LEFT     = 3;
    sensor_data.LIDAR_Obstacle_BACK     = 2;

    drive_motor_stop_Expect();
    master_dont_steer_Expect();
    start_obstacle_avoidance();
}

void test_start_obstacle_avoidance__check_rear_obstacle_else(void)
{
    sensor_data.SENSOR_FRONT_cm         = 99;
    sensor_data.LIDAR_Obstacle_RIGHT    = 3;
    sensor_data.LIDAR_Obstacle_LEFT     = 3;
    sensor_data.LIDAR_Obstacle_BACK     = 3;

    drive_motor_rev_slow_Expect();
    master_dont_steer_Expect();
    start_obstacle_avoidance();
}

void test_master_service_can_msgs__can_rx_true_bridge_msg(void)
{
    can_msg.msg_id = 103;
    service_bridge_msg_Expect();
    CAN_rx_ExpectAndReturn(can, NULL, 0, true);
    CAN_rx_IgnoreArg_msg();

    CAN_rx_ExpectAndReturn(can, NULL, 0, false);
    CAN_rx_IgnoreArg_msg();

    CAN_rx_ReturnThruPtr_msg(&can_msg);

    master_service_can_msgs();
    TEST_ASSERT_EQUAL(103,can_msg_hdr.mid);
    master_mia_handler();
}

void test_master_service_can_msgs__can_rx_true_sensor_msg(void)
{
    can_msg.msg_id = 105;
    service_sensor_msg_Expect();
    CAN_rx_ExpectAndReturn(can, NULL, 0, true);
    CAN_rx_IgnoreArg_msg();

    CAN_rx_ExpectAndReturn(can, NULL, 0, false);
    CAN_rx_IgnoreArg_msg();

    CAN_rx_ReturnThruPtr_msg(&can_msg);

    master_service_can_msgs();
    TEST_ASSERT_EQUAL(105,can_msg_hdr.mid);
//    master_mia_handler();
}

void test_master_service_can_msgs__can_rx_true_gps_msg(void)
{
    can_msg.msg_id = 108;
    service_gps_msg_Expect();
    CAN_rx_ExpectAndReturn(can, NULL, 0, true);
    CAN_rx_IgnoreArg_msg();

    CAN_rx_ExpectAndReturn(can, NULL, 0, false);
    CAN_rx_IgnoreArg_msg();

    CAN_rx_ReturnThruPtr_msg(&can_msg);

    master_service_can_msgs();
    TEST_ASSERT_EQUAL(108,can_msg_hdr.mid);
    master_mia_handler();
}

void test_master_service_can_msgs__can_rx_true_compass_msg(void)
{
    can_msg.msg_id = 109;
    service_compass_msg_Expect();
    CAN_rx_ExpectAndReturn(can, NULL, 0, true);
    CAN_rx_IgnoreArg_msg();

    CAN_rx_ExpectAndReturn(can, NULL, 0, false);
    CAN_rx_IgnoreArg_msg();

    CAN_rx_ReturnThruPtr_msg(&can_msg);

    master_service_can_msgs();
    TEST_ASSERT_EQUAL(109,can_msg_hdr.mid);
    master_mia_handler();
}

void test_master_service_can_msgs__can_rx_true_sensor_hbt_msg(void)
{
    can_msg.msg_id = 111;
    service_sensor_hbt_msg_Expect();
    CAN_rx_ExpectAndReturn(can, NULL, 0, true);
    CAN_rx_IgnoreArg_msg();

    CAN_rx_ExpectAndReturn(can, NULL, 0, false);
    CAN_rx_IgnoreArg_msg();

    CAN_rx_ReturnThruPtr_msg(&can_msg);

    master_service_can_msgs();
    TEST_ASSERT_EQUAL(111,can_msg_hdr.mid);
    master_mia_handler();
}

void test_master_service_can_msgs__can_rx_true_motor_hbt_msg(void)
{
    can_msg.msg_id = 112;
    service_motor_hbt_msg_Expect();
    CAN_rx_ExpectAndReturn(can, NULL, 0, true);
    CAN_rx_IgnoreArg_msg();

    CAN_rx_ExpectAndReturn(can, NULL, 0, false);
    CAN_rx_IgnoreArg_msg();

    CAN_rx_ReturnThruPtr_msg(&can_msg);

    master_mia_handler();
    master_service_can_msgs();

    TEST_ASSERT_EQUAL(112,can_msg_hdr.mid);
}

void test_master_service_can_msgs__can_rx_true_gps_hbt_msg(void)
{
    can_msg.msg_id = 113;

    service_gps_hbt_msg_Expect();
    CAN_rx_ExpectAndReturn(can, NULL, 0, true);
    CAN_rx_IgnoreArg_msg();

    CAN_rx_ExpectAndReturn(can, NULL, 0, false);
    CAN_rx_IgnoreArg_msg();

    CAN_rx_ReturnThruPtr_msg(&can_msg);

    master_mia_handler();
    master_service_can_msgs();

    TEST_ASSERT_EQUAL(113,can_msg_hdr.mid);
}

void test_master_service_can_msgs__can_rx_true_bridge_hbt_msg(void)
{
    can_msg.msg_id = 114;

    service_bridge_hbt_msg_Expect();
    CAN_rx_ExpectAndReturn(can, NULL, 0, true);
    CAN_rx_IgnoreArg_msg();

    CAN_rx_ExpectAndReturn(can, NULL, 0, false);
    CAN_rx_IgnoreArg_msg();

    CAN_rx_ReturnThruPtr_msg(&can_msg);

    master_mia_handler();
    master_service_can_msgs();

    TEST_ASSERT_EQUAL(114,can_msg_hdr.mid);
}

void test_master_service_can_msgs__can_rx_true_default(void)
{
    can_msg.msg_id = 120;

    CAN_rx_ExpectAndReturn(can, NULL, 0, true);
    CAN_rx_IgnoreArg_msg();

    CAN_rx_ExpectAndReturn(can, NULL, 0, false);
    CAN_rx_IgnoreArg_msg();

    CAN_rx_ReturnThruPtr_msg(&can_msg);
    master_mia_handler();
    master_service_can_msgs();

    TEST_ASSERT_EQUAL(120,can_msg_hdr.mid);
}

void test_master_mia_handler(void)
{
    //bridge node
    setLED_Expect(1, true);
    bridge_data.mia_info.is_mia = false;
    bridge_data.mia_info.mia_counter_ms = 3000;
    master_mia_handler();

    //sensor node
    setLED_Expect(2, true);
    sensor_data.mia_info.is_mia = false;
    sensor_data.mia_info.mia_counter_ms = 3000;
    master_mia_handler();

    //gps location
    setLED_Expect(3, true);
    gps_data.mia_info.is_mia = false;
    gps_data.mia_info.mia_counter_ms = 3000;
    master_mia_handler();

    //compass
    setLED_Expect(4, true);
    compass_data.mia_info.is_mia = false;
    compass_data.mia_info.mia_counter_ms = 3000;
    master_mia_handler();

    //sensor hbt
    sensor_hbt.mia_info.is_mia = false;
    sensor_hbt.mia_info.mia_counter_ms = 3000;
    master_mia_handler();


    //motor hbt
    motor_hbt.mia_info.is_mia = false;
    motor_hbt.mia_info.mia_counter_ms = 3000;
    master_mia_handler();


    //gps hbt


    //bridge hbt
}

void test_master_send_command_to_motor_module__true(void)
{
    car_control.MOTOR_DRIVE_cmd = MOTOR_FORWARD;
    car_control.MOTOR_STEER_cmd = MOTOR_DONT_STEER;
    car_control.MOTOR_kph = MOTOR_SLOW_KPH;
    CAN_tx_ExpectAndReturn(can, &can_msg, 0, true);
    master_send_command_to_motor_module();
}

void test_master_send_debug_msg(void)
{
//    MASTER_DEBUG_t master_dbg_msg = {0};
//    master_dbg_msg.IO_DEBUG_CAN_init = true;;
//    master_dbg_msg.IO_DEBUG_HBT_FROM_ALL_CONTR = true;
//    master_dbg_msg.IO_DEBUG_drive_mode = false;
//    master_dbg_msg.IO_DEBUG_bus_off = false;

    CAN_tx_ExpectAndReturn(can, &can_msg, 0, true);
    master_send_debug_msg();
}

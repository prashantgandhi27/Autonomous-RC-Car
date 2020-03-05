#include <stdio.h>
#include <stdint.h>
#include "unity.h"

#include "Sensor_CAN.h"
#include "generated_can.h"
//#include "Mockultrasonic_sensor.h"
#include "Mockcan.h"
#include "Mockc_io.h"

void setUp(void)
{
   
}
void tearDown(void)
{

}

void test_sensor_can_init(void)
{	
	CAN_init_ExpectAndReturn(can1, 100, 10, 10, 0, 0, 1);
	CAN_reset_bus_Expect(can1);
	TEST_ASSERT_TRUE(sensor_can_init());
	
	//CAN_init_ExpectAndReturn(can1, 100, 10, 10, 0, 0, 0);
	//TEST_ASSERT_FALSE(sensor_can_init());
}

 void test_sensor_send_data(void)
{
	//can_msg_t can_msg = {0};
	//SENSOR_NODE_t sensor_cmd;
   // sensor_lv_max_sonar_t send_ultrasonic_data;
	
	CAN_tx_ExpectAndReturn(0,NULL,0,1);
    CAN_tx_IgnoreArg_msg();
	TEST_ASSERT_TRUE(sensor_send_data());
}

void test_send_sensor_debug_msg(void)
{
    CAN_tx_ExpectAndReturn(0,NULL,0,1);
    CAN_tx_IgnoreArg_msg();
    send_sensor_debug_msg();

    CAN_tx_ExpectAndReturn(0,NULL,0,0);
    CAN_tx_IgnoreArg_msg();
    send_sensor_debug_msg();
}

void test_transmit_heartbeat_on_can(void)
{
    setLED_Expect(4,1);
    CAN_tx_ExpectAndReturn(0,NULL,0,1);
    CAN_tx_IgnoreArg_msg();
    transmit_heartbeat_on_can();

    setLED_Expect(4,1);
    CAN_tx_ExpectAndReturn(0,NULL,0,0);
    CAN_tx_IgnoreArg_msg();
    transmit_heartbeat_on_can();
}

void test_sensor_CAN_turn_on_bus_if_bus_off(void)
{
    CAN_is_bus_off_ExpectAndReturn(0,true);
    CAN_reset_bus_Expect(0);
    CAN_bypass_filter_accept_all_msgs_Expect();
    sensor_CAN_turn_on_bus_if_bus_off();

    CAN_is_bus_off_ExpectAndReturn(0,false);
    sensor_CAN_turn_on_bus_if_bus_off();
}
 
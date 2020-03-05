#include "unity.h"
#include "can_tx.h"
#include <stdbool.h>
#include <stdio.h>
#include "Mockcan.h"
#include "generated_can.h"
#include "Mockgps.h"
#include "Mockc_io.h"
#include "Mockcompass.h"

void setUp(void) {

}
void tearDown(void) {
}

void test_can_init(void)
{
   CAN_init_ExpectAndReturn(0, 100, 100, 100, NULL, NULL,true);
   CAN_reset_bus_Expect(0);
   TEST_ASSERT_TRUE(can_init());

   CAN_init_ExpectAndReturn(0, 100, 100, 100, NULL, NULL,false);
   TEST_ASSERT_FALSE(can_init());
  // CAN_init_ExpectAnyArgsAndReturn(false);
  // TEST_ASSERT_FALSE(can_init());
}

void test_transmit_gps_data_on_can(void)
{
    getLatitude_ExpectAndReturn(37.22222);
    getLongitude_ExpectAndReturn(-121.8888);
    CAN_tx_ExpectAndReturn(0,NULL,0,1);
    CAN_tx_IgnoreArg_msg();
    transmit_gps_data_on_can();

    getLatitude_ExpectAndReturn(0.0);
    getLongitude_ExpectAndReturn(-121.8888);
    CAN_tx_ExpectAndReturn(0,NULL,0,0);
    CAN_tx_IgnoreArg_msg();
    transmit_gps_data_on_can();
}

void test_transmit_compass_data_on_can(void)
{
    Compass_Get_Bearing_Angle_ExpectAndReturn(332.4);
    CAN_tx_ExpectAndReturn(0,NULL,0,1);
    CAN_tx_IgnoreArg_msg();
    transmit_compass_data_on_can();

    Compass_Get_Bearing_Angle_ExpectAndReturn(0.0);
    CAN_tx_ExpectAndReturn(0,NULL,0,0);
    CAN_tx_IgnoreArg_msg();
    transmit_compass_data_on_can();
}

void test_transmit_debug_data_on_can(void)
{
    CAN_tx_ExpectAndReturn(0,NULL,0,1);
    CAN_tx_IgnoreArg_msg();
    transmit_debug_data_on_can();

    CAN_tx_ExpectAndReturn(0,NULL,0,0);
    CAN_tx_IgnoreArg_msg();
    transmit_debug_data_on_can();
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

void test_check_bus_off(void)
{
    CAN_is_bus_off_ExpectAndReturn(0,true);
    CAN_reset_bus_Expect(0);
    CAN_bypass_filter_accept_all_msgs_Expect();
    check_bus_off();

    CAN_is_bus_off_ExpectAndReturn(0,false);
    check_bus_off();
}

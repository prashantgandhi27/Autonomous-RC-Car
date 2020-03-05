
#include <stdio.h>
#include "unity.h"
#include "Lidar_funcs.h"
#include "Mockc_uart2.h"
#include "MockRPLidar.h"
#include "MockSensor_CAN.h"

void test_Lidar_init(void)
{
	 rplidar_response_device_health_t get_healthinfo;

     Uart_init_for_sensor_Expect();
	 
	 getHealth_ExpectAndReturn(&get_healthinfo, 500, 0);
	 getHealth_IgnoreArg_healthinfo();
	 getHealth_ReturnThruPtr_healthinfo(&get_healthinfo);
	 
	 startScan_ExpectAndReturn(500, 0);
	 
	 TEST_ASSERT_TRUE(Lidar_init());
}

void test_Lidar_get_data(void)
{
	rplidar_response_measurement_node_t raw_response_node;
	RPLidarMeasurement refined_response;
	
	for(int i=0;i<360;i++)
    {
		waitPoint_ExpectAndReturn(&raw_response_node, 500, &refined_response, 1);
		waitPoint_IgnoreArg_node();
		waitPoint_IgnoreArg_measurement();
		waitPoint_ReturnThruPtr_node(&raw_response_node);
		waitPoint_ReturnThruPtr_measurement(&refined_response);

	}
	Lidar_get_data();
}

void test_Lidar_parse_data(void)
{
	RPLidarMeasurement refined_response_buff[360];
	RPLidarRotation singleRotation;
	divideAngle_Expect(refined_response_buff, 0,&singleRotation);
	divideAngle_IgnoreArg_length();
	divideAngle_IgnoreArg_angle_value();
	divideAngle_IgnoreArg_rot();
	divideAngle_ReturnThruPtr_angle_value(refined_response_buff);
	divideAngle_ReturnThruPtr_rot(&singleRotation);
   Lidar_parse_data();
}

void test_Lidar_send_data(void)
{
	//RPLidarRotation sr;
	sensor_send_data_ExpectAndReturn(1);
	//sensor_send_data_IgnoreArg_singleRotation();
	//sensor_send_data_ReturnThruPtr_singleRotation(&sr);
   Lidar_send_data();
}
	
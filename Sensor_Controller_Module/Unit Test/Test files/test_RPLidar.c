
#include <stdio.h>
#include "unity.h"
#include "RPLidar.h"
#include "generated_can.h"
#include "Mockc_uart2.h"


 void test_uart_init_for_sensor(void)
{
	uart2_instance_ExpectAnyArgs();
	uart2_init_Expect(115200, 50, 50);
	Uart_init_for_sensor();
} 

 void test__sendCommand(void)
{
	uart2_put_char_ExpectAndReturn(0xA5, 0, 1);
    uart2_put_char_ExpectAndReturn(123, 0, 1);
	TEST_ASSERT_FALSE(_sendCommand(123,0,0));
}
	
void test__waitResponseHeader(void)
{
	char received;
	uart2_get_char_ExpectAndReturn(&received, 0, 1);
	uart2_get_char_IgnoreArg_pInputChar();
	uart2_get_char_ReturnThruPtr_pInputChar(&received);
	TEST_ASSERT_TRUE(_waitResponseHeader(0,0));
	
}
void test_startScan(void)
{

	//uint32_t ans = _sendCommand(0x20,NULL,0);
	TEST_ASSERT_EQUAL_HEX(0x80000000,_sendCommand(0x20,0,0));
	TEST_ASSERT_EQUAL_HEX(0x80000000,startScan(0));
	
}

void test_divideDistance(void)
{
	TEST_ASSERT_EQUAL(1,divideDistance(290.00));
	TEST_ASSERT_EQUAL(2,divideDistance(501.00));
	TEST_ASSERT_EQUAL(3,divideDistance(800.00));
	TEST_ASSERT_EQUAL(4,divideDistance(1100.00));
	TEST_ASSERT_EQUAL(5,divideDistance(1300.00));
	TEST_ASSERT_EQUAL(6,divideDistance(1600.00));
	TEST_ASSERT_EQUAL(7,divideDistance(23628612.00));
	
	TEST_ASSERT_EQUAL(0,divideDistance(240.00));
}


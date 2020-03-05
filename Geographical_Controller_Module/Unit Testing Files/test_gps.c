#include "unity.h"
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <stdbool.h>
#include "Mockc_io.h"
#include "Mockutilities.h"
#include "Mockc_uart2.h"
#include "Mockc_uart_dev.h"
#include "gps.h"

void setUp(void) {

}
void tearDown(void) {
}

void test_create_gps_config(void)
{
    cputline_Expect("$PMTK251,57600*2C",30);
    cputline_Expect("$PMTK314,0,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0*29",10);
    cputline_Expect("$PMTK220,100*2F", 10);
    cputline_Expect("$PMTK300,200,0,0,0,0*2F", 10);
    create_gps_config();
}

//UART INIT
void test_init_gps(void)
{
    Clear_Display_Expect();      /* Clear the display */
    cUart2_init_ExpectAndReturn(57600, 120, 60,1);      /* Init baud rate */
    cputline_Expect("$PMTK251,57600*2C",30);
    cputline_Expect("$PMTK314,0,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0*29",10);
    cputline_Expect("$PMTK220,100*2F", 10);
    cputline_Expect("$PMTK300,200,0,0,0,0*2F", 10);
    init_gps();
}

void test_gps_rx(void)
{
    char buffer[120] = "$GPRMC,084128.00,V,,,,,,,160419,,,N*71";
    cgets_ExpectAndReturn(NULL, 120, 50,1);
    cgets_IgnoreArg_pBuff();
    cgets_ReturnThruPtr_pBuff(buffer);
    setLED_Expect(3,1);
    gps_rx();
}

void test_gps_parse_data(void)
{
    // TEST_ASSERT_FALSE()

    setLCD_LEFT_Expect('0');
    setLCD_Right_Expect('0');
    setLED_Expect(2,0);
    TEST_ASSERT_EQUAL(0,gps_parse_data());
}

void test_getLatitude(void)
{
    TEST_ASSERT_EQUAL_FLOAT(0,getLatitude());
}

void test_getLongitude(void)
{
    TEST_ASSERT_EQUAL_FLOAT(0,getLongitude());
}

void test_check_for_data_on_gps(void)
{
    char buffer[120] = "$GPRMC,084128.00,V,,,,,,,160419,,,N*71";
    cgets_ExpectAndReturn(NULL, 120, 50,1);
    cgets_IgnoreArg_pBuff();
    cgets_ReturnThruPtr_pBuff(buffer);
    setLED_Expect(3,1);
    delay_ms_Expect(1);

    setLCD_LEFT_Expect('0');
    setLCD_Right_Expect('0');
    setLED_Expect(2,0);
    check_for_data_on_gps();
}


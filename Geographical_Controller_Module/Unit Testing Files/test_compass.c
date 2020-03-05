#include "unity.h"
#include "compass.h"
#include "Mockc_i2c_base.h"
#include "Mockc_io.h"
#include "Mockutilities.h"
#include <stdio.h>

void setUp(void) {

}
void tearDown(void) {
}

void test_Compass_Get_Bearing_Angle(void)
{
    uint8_t buffer[2] = {1,2};
    I2C_ReadRegisters_ExpectAndReturn(0xC0,0x02,NULL,2,1);
    I2C_ReadRegisters_IgnoreArg_pData();
    I2C_ReadRegisters_ReturnThruPtr_pData(buffer);
    TEST_ASSERT_FLOAT_WITHIN(1, 25.8,Compass_Get_Bearing_Angle());

    I2C_ReadRegisters_ExpectAndReturn(0xC0,0x02,NULL,2,0);
    I2C_ReadRegisters_IgnoreArg_pData();
    I2C_ReadRegisters_ReturnThruPtr_pData(buffer);
    TEST_ASSERT_EQUAL_FLOAT(0.0,Compass_Get_Bearing_Angle());
}

void test_Start_Compass_Calibration(void)
{
    uint8_t buffer[3] = { 0xF0,0xF5,0xF6 };
    I2C_writeRegisters_ExpectAndReturn(0xC0, 0x00,&buffer[0],1,1);
    delay_ms_Expect(30);

    I2C_writeRegisters_ExpectAndReturn(0xC0, 0x00,&buffer[1],1,1);
    delay_ms_Expect(30);

    I2C_writeRegisters_ExpectAndReturn(0xC0, 0x00,&buffer[2],1,1);
    Start_Compass_Calibration();
}

void test_Stop_Compass_Calibration(void)
{
    uint8_t buffer[1] = { 0xF8 };
    I2C_writeRegisters_ExpectAndReturn(0xC0, 0x00,&buffer[0],1,1);
    Stop_Compass_Calibration();
}

void test_Set_Default_Calibration(void)
{
    uint8_t buffer[3] = { 0x20,0x2A,0x60 };

    I2C_writeRegisters_ExpectAndReturn(0xC0, 0x00,&buffer[0],1,1);
    delay_ms_Expect(30);

    I2C_writeRegisters_ExpectAndReturn(0xC0, 0x00,&buffer[1],1,1);
    delay_ms_Expect(30);

    I2C_writeRegisters_ExpectAndReturn(0xC0, 0x00,&buffer[2],1,1);
    Set_Default_Calibration();
}

void test_get_compass_data(void)
{
    uint8_t buffer[8] = {0xF0,0xF5,0xF6,0xF8,0x20,0x2A,0x60};
    uint8_t bufferAngle[2] = {1,2};

    switch_detect_ExpectAndReturn(1);
    setLED_Expect(1,1);
    I2C_writeRegisters_ExpectAndReturn(0xC0, 0x00,&buffer[0],1,1);
    delay_ms_Expect(30);
    I2C_writeRegisters_ExpectAndReturn(0xC0, 0x00,&buffer[1],1,1);
    delay_ms_Expect(30);
    I2C_writeRegisters_ExpectAndReturn(0xC0, 0x00,&buffer[2],1,1);
    get_compass_data();

    switch_detect_ExpectAndReturn(2);
    setLED_Expect(2,1);
    I2C_writeRegisters_ExpectAndReturn(0xC0, 0x00,&buffer[3],1,1);
    get_compass_data();

    switch_detect_ExpectAndReturn(4);
    setLED_Expect(3,1);
    I2C_writeRegisters_ExpectAndReturn(0xC0, 0x00,&buffer[4],1,1);
    delay_ms_Expect(30);
    I2C_writeRegisters_ExpectAndReturn(0xC0, 0x00,&buffer[5],1,1);
    delay_ms_Expect(30);
    I2C_writeRegisters_ExpectAndReturn(0xC0, 0x00,&buffer[6],1,1);
    get_compass_data();

    switch_detect_ExpectAndReturn(8);
    setLED_Expect(4,1);
    I2C_ReadRegisters_ExpectAndReturn(0xC0,0x02,NULL,2,1);
    I2C_ReadRegisters_IgnoreArg_pData();
    I2C_ReadRegisters_ReturnThruPtr_pData(bufferAngle);
    get_compass_data();

    switch_detect_ExpectAndReturn(8);
    setLED_Expect(4,1);
    I2C_ReadRegisters_ExpectAndReturn(0xC0,0x02,NULL,2,0);
    I2C_ReadRegisters_IgnoreArg_pData();
    I2C_ReadRegisters_ReturnThruPtr_pData(bufferAngle);
    get_compass_data();

    switch_detect_ExpectAndReturn(5);
    get_compass_data();
}

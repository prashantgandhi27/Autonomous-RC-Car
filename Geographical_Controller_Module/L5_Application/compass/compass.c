#include "compass.h"
#include "c_io.h"
#include "utilities.h"
#include "c_i2c_base.h"
#include <stdio.h>

float Compass_Get_Bearing_Angle(void)
{
    int bearing_int = 0;
    float bearing_float = 0;
    uint8_t buffer[2] = { 0 };

    if (I2C_ReadRegisters(COMPASS_I2C_ADDR, COMPASS_BEARING_16BIT_REG_ADDR, &buffer[0], READ_TWO_REGISTERS))
    {
        setLED_gpio(3,true);
        bearing_int = buffer[0];
        bearing_int <<= 8;
        bearing_int += buffer[1];
        bearing_float = (float)(bearing_int/10.00);
       // printf("%f\n",bearing_float);
        return bearing_float;
    }
    return 0;
}

void Start_Compass_Calibration(void)
{
    uint8_t buffer[3] = { 0xF0,0xF5,0xF6 };
    I2C_writeRegisters(COMPASS_I2C_ADDR, COMMAND_REG_ADDR, &buffer[0], READ_ONE_REGISTER);
    delay_ms(30);
    I2C_writeRegisters(COMPASS_I2C_ADDR, COMMAND_REG_ADDR, &buffer[1], READ_ONE_REGISTER);
    delay_ms(30);
    I2C_writeRegisters(COMPASS_I2C_ADDR, COMMAND_REG_ADDR, &buffer[2], READ_ONE_REGISTER);
}


void Stop_Compass_Calibration(void)
{
    uint8_t buffer[1] = { 0 };
    buffer[0] = 0xF8;
    I2C_writeRegisters(COMPASS_I2C_ADDR, COMMAND_REG_ADDR, &buffer[0], READ_ONE_REGISTER);
}

void Set_Default_Calibration(void)
{
    uint8_t buffer[1] = { 0 };
    buffer[0] = 0x20;
    I2C_writeRegisters(COMPASS_I2C_ADDR, COMMAND_REG_ADDR, &buffer[0], READ_ONE_REGISTER);
    delay_ms(30);
    buffer[0] = 0x2A;
    I2C_writeRegisters(COMPASS_I2C_ADDR, COMMAND_REG_ADDR, &buffer[0], READ_ONE_REGISTER);
    delay_ms(30);
    buffer[0] = 0x60;
    I2C_writeRegisters(COMPASS_I2C_ADDR, COMMAND_REG_ADDR, &buffer[0], READ_ONE_REGISTER);
}

void get_compass_data(void)
{
    switch(switch_detect())
    {
        case sw1:
            setLED(1,1);
            Start_Compass_Calibration();
            break;
        case sw2:
            setLED(2,1);
            Stop_Compass_Calibration();
            break;
        case sw3:
            setLED(3,1);
            Set_Default_Calibration();
            break;
        case sw4:
            setLED(4,1);
            Compass_Get_Bearing_Angle();
            break;
        default:
        break;
    }
}


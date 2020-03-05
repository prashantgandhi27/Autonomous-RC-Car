/*
 * c_io.cpp
 *
 *  Created on: Mar 4, 2019
 *      Author: Aakash
 */
#include "c_io.h"
#include "io.hpp"
#include "defines.h"
#include "gpio.hpp"

#define D_19    P1_20
#define D_18    P1_19
#define D_17    P0_30
#define D_16    P0_29

static GPIO bridge_hbt_LED(D_19);
static GPIO sensor_hbt_LED(D_18);
static GPIO motor_hbt_LED(D_17);
static GPIO geo_hbt_LED(D_16);

void clearLedDisplay(void)
{
    LD.clear();
}

void setLED(uint8_t ledNum, bool on)
{
    LE.set(ledNum, on);
}

void dispDataOnLedDisplay(uint8_t byte_to_display)
{
    LD.setNumber(byte_to_display);
}

void dispDataSetRightDigit(uint8_t alpha)
{
    LD.setRightDigit(alpha);
}

void dispDataSetLeftDigit(char alpha)
{
    LD.setLeftDigit(alpha);
}

uint8_t getLSDData(void)
{
    return (LS.getPercentValue());
}

bool getSwitch(int num)
{
    return SW.getSwitch(num);
}

void init_Ext_LED_as_output(void)
{
    sensor_hbt_LED.setAsOutput();
    bridge_hbt_LED.setAsOutput();
    motor_hbt_LED.setAsOutput();
    geo_hbt_LED.setAsOutput();
}


void set_Ext_LED(int led_num, bool status)
{
    switch (led_num)
    {
        case BRIDGE_LED:
            bridge_hbt_LED.set(status);
            break;
        case SENSOR_LED:
            sensor_hbt_LED.set(status);
            break;
        case MOTOR_LED:
            motor_hbt_LED.set(status);
            break;
        case GEO_LED:
            geo_hbt_LED.set(status);
            break;
        default:
            break;
    }
}

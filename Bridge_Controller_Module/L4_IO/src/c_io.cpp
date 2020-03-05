/*
 * c_io.cpp
 *
 *  Created on: Mar 4, 2019
 *      Author: vidushi
 */
#include "c_io.h"
#include "io.hpp"
#include "gpio.hpp"

static GPIO bluetoothStatus(P0_30);
static GPIO bridgeHeartBeat(P0_29);

void setLED(uint8_t ledNum, bool on)
{
    LE.set(ledNum, on);
}

void init_gpio(void)
{
    bluetoothStatus.setAsOutput();
    bridgeHeartBeat.setAsOutput();
}

void setLED_gpio(int led_num,bool on)
{
    switch(led_num)
    {
        case 1:
        {
            bluetoothStatus.set(on);
            break;
        }
        case 2:
        {
            bridgeHeartBeat.set(on);
            break;
        }
    }
}

void setLCD_Display(char num)
{
    LD.setNumber(num);
}

void setLCD_LEFT(char num)
{
    LD.setLeftDigit(num);
}

void setLCD_Right(char num)
{
    LD.setRightDigit(num);
}

void Clear_Display(void)
{
    LD.clear();
}

uint8_t switch_detect(void)
{
    uint8_t switches = SW.getSwitchValues();
    return switches;
}

bool switch_function(int number)
{
    bool res = SW.getSwitch(number);
    return res;
}


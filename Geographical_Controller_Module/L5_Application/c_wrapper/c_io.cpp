#include "c_io.h"
#include "io.hpp"
#include "gpio.hpp"


static GPIO GPSFIX(P0_30);
static GPIO GPSHeartBeat(P0_29);
static GPIO Compass(P1_19);

void init_gpio(void)
{
    GPSFIX.setAsOutput();
    GPSHeartBeat.setAsOutput();
    Compass.setAsOutput();
}

void setLED_gpio(int led_num,bool on)
{
    switch(led_num)
    {
        case 1:
            GPSFIX.set(on);
            break;
        case 2:
            GPSHeartBeat.set(on);
            break;
        case 3:
            Compass.set(on);
            break;

    }
}

void setLED(uint8_t ledNum, bool on)
{
    LE.set(ledNum, on);
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

uint8_t switch_detect()
{
    uint8_t switches = SW.getSwitchValues();
    return switches;
}

bool switch_function(int number)
{
    bool res = SW.getSwitch(number);
    return res;
}


/*
 * c_gpio.cpp
 *
 *  Created on: Apr 9, 2019
 *      Author: halak
 */

#include "c_gpio.h"

GPIO *gpio;
#define D_11    P0_29
#define D_12    P0_30
#define D_13    P1_19
#define D_14    P1_20
#define D_15    P1_22

static GPIO Master_hbt_LED(D_15);
static GPIO Left_Obstacle_LED(D_12);
static GPIO Right_Obstacle_LED(D_14);
static GPIO Front_Obstacle_LED(D_13);
static GPIO Back_Obstacle_LED(D_11);


void setPin()
{
     gpio = new GPIO(P2_2);
}

void setInput()
{
    gpio->setAsInput();
}

void setOutput()
{
    gpio->setAsOutput();
}

void SET(bool check)
{
    gpio->set(check);
}


void init_Ext_LED_as_output(void)
{
    Master_hbt_LED.setAsOutput();
    Left_Obstacle_LED.setAsOutput();
    Right_Obstacle_LED.setAsOutput();
    Front_Obstacle_LED.setAsOutput();
    Back_Obstacle_LED.setAsOutput();
}


void set_Ext_LED(int led_num, bool status)
{
    switch (led_num)
    {
        case D_11:
            Back_Obstacle_LED.set(status);
            break;
        case D_12:
            Left_Obstacle_LED.set(status);
            break;
        case D_13:
            Front_Obstacle_LED.set(status);
            break;
        case D_14:
            Right_Obstacle_LED.set(status);
            break;
        case D_15:
            Master_hbt_LED.set(status);
            break;
        default:
            break;
    }
}


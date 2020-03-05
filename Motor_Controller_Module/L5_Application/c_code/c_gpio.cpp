/*
 * c_gpio.cpp
 *
 *  Created on: Apr 9, 2019
 *      Author: Jay
 */

#include "c_gpio.h"
#include "gpio.hpp"

GPIO *gpio_2_5;
GPIO *gpio_0_29;
GPIO *gpio_0_30;
GPIO *gpio_1_19;
GPIO *gpio_1_20;

void GPIO_interrupt(void) {
     gpio_2_5 = new GPIO(P2_5);
     gpio_2_5->setAsInput();
}

void init_LEDs(void) {
    gpio_0_29 = new GPIO(P0_29);
    gpio_0_30 = new GPIO(P0_30);
    gpio_1_19 = new GPIO(P1_19);
    gpio_1_20 = new GPIO(P1_20);

    gpio_0_29->setAsOutput();
    gpio_0_30->setAsOutput();
    gpio_1_19->setAsOutput();
    gpio_1_20->setAsOutput();

    gpio_0_29->setLow();
    gpio_0_30->setLow();
    gpio_1_19->setLow();
    gpio_1_20->setLow();
}

void motor_set_led(MOTOR_LED port_pin_, bool sts) {
    if (sts)
        switch (port_pin_) {
        case _p0_29:
            gpio_0_29->setHigh();
            break;
        case _p0_30:
            gpio_0_30->setHigh();
            break;
        case _p1_19:
            gpio_1_19->setHigh();
            break;
        case _p1_20:
            gpio_1_20->setHigh();
            break;
        default:
            gpio_0_29->setLow();
            gpio_0_30->setLow();
            gpio_1_19->setLow();
            gpio_1_20->setLow();
    }
    else
        switch (port_pin_) {
        case _p0_29:
            gpio_0_29->setLow();
            break;
        case _p0_30:
            gpio_0_30->setLow();
            break;
        case _p1_19:
            gpio_1_19->setLow();
            break;
        case _p1_20:
            gpio_1_20->setLow();
            break;
        default:
            gpio_0_29->setLow();
            gpio_0_30->setLow();
            gpio_1_19->setLow();
            gpio_1_20->setLow();
        }
}

/*
 * c_switch.cpp
 *
 *  Created on: 04-Mar-2019
 *      Author: Vatsalm93
 */

#include <stdint.h>
#include <stdbool.h>
#include "switches.hpp"
#include "c_switch.h"


bool c_switch_init(void)
{
    return Switches::getInstance().init();
}

bool c_getSwitch(int num)
{
    return Switches::getInstance().getSwitch(1);
}



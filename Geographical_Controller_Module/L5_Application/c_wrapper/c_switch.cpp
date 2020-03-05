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



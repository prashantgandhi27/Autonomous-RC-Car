/*
 * lcd_task.cpp
 *
 *  Created on: Apr 27, 2019
 *      Author: Jay
 */

#include "scheduler_task.hpp"
#include "tasks.hpp"
#include "c_code/lcd.h"

lcd_task :: lcd_task(uint8_t priority):scheduler_task("lcd_task", 2000, priority){
}

bool lcd_task :: init(void){
    lcd_init();
    return true;
}
bool lcd_task :: run(void *p){
    lcd_screen_query();

    return true;
}



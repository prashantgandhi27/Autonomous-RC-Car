/*
 * speed_calculator.c
 *
 *  Created on: Apr 19, 2019
 *      Author: Jay
 */

#include "speed_calculator.h"

uint16_t pulse = 0;
float speed_km_hr = 0;

void eint3_handler(void){
    pulse++;
}

float get_speed(void) {
    return speed_km_hr;
}

float calculate_speed(void) {
    uint16_t rotation_per_sec = pulse;
    pulse = 0;
    speed_km_hr = (circumference * rotation_per_sec) / (2.7);
    return speed_km_hr;
}

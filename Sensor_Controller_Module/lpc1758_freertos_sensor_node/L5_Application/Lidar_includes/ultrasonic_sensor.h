/*
 * ultrasonic_sensor.h
 *
 *  Created on: Apr 10, 2019
 *      Author: halak
 */

#ifndef ULTRASONIC_SENSOR_H_
#define ULTRASONIC_SENSOR_H_
#ifdef __cplusplus
extern "C"{
#endif

#include <stdint.h>
#include <stdbool.h>

typedef struct
{
    float distance;
    uint32_t start_echo_time;
    uint32_t end_echo_time;
}sensor_lv_max_sonar_t;

void sensor_cntlr_init();
void sensor_start_scan();
void sensor_stop_scan();

#ifdef __cplusplus
}
#endif
#endif /* ULTRASONIC_SENSOR_H_ */

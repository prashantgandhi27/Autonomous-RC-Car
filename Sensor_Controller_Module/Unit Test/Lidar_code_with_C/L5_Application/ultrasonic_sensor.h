/*
 * ultrasonic_sensor.h
 *
 *  Created on: Apr 25, 2019
 *      Author: User
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

void sensor_cntlr_init(void);
void sensor_start_scan(void);
void sensor_stop_scan(void);

#ifdef __cplusplus
}
#endif

#endif /* ULTRASONIC_SENSOR_H_ */

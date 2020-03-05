/*
 * steer_processor.h
 *
 *  Created on: Feb 24, 2019
 *      Author: halak
 */

#ifndef STEER_PROCESSOR_H_
#define STEER_PROCESSOR_H_

#include "steering.h"
#include <stdio.h>
#include <stdint.h>

/**
 * Assume that a threshold value is 50cm
 * Objective is to steer left or right based on the left and right sensor values
 * Example: If left sensor is 10cm, and right is 70cm, then we should call steer_right()
 */
void steer_processor(int32_t left_sensor_cm, int32_t right_sensor_cm);


#endif /* STEER_PROCESSOR_H_ */

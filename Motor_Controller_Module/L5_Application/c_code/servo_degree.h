/*
 * servo_degree.h
 *
 *  Created on: May 4, 2019
 *      Author: Jay
 */

#ifndef SERVO_DEGREE_H_
#define SERVO_DEGREE_H_

#include <stdint.h>
#include <stdbool.h>
#include "generated_can.h"

#ifdef __cplusplus
extern "C" {
#endif

float get_servo_angle(CAR_CONTROL_t * servo);

#ifdef __cplusplus
}
#endif

#endif /* SERVO_DEGREE_H_ */

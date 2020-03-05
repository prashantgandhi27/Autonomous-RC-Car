/*
 * c_pwm.h
 *
 *  Created on: Apr 17, 2019
 *      Author: halak
 */

#ifndef C_PWM_H_
#define C_PWM_H_

#ifdef __cplusplus
extern "C" {
#endif

#include <stdint.h>
#include <stdbool.h>

void init_pwm(unsigned int freq);
void set_pwm_value(float percent);

#ifdef __cplusplus
}
#endif



#endif /* C_PWM_H_ */

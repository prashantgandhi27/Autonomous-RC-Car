/*
 * c_gpio.h
 *
 *  Created on: Apr 9, 2019
 *      Author: halak
 */

#ifndef C_GPIO_H_
#define C_GPIO_H_

#include <stdbool.h>
#include <stdint.h>

#ifdef __cplusplus
extern "C" {
#endif

typedef enum {
    _p0_29,
    _p0_30,
    _p1_19,
    _p1_20,
} MOTOR_LED;

void GPIO_interrupt(void);
void init_LEDs(void);
void motor_set_led(MOTOR_LED port_pin, bool sts);

#ifdef __cplusplus
}
#endif

#endif /* C_GPIO_H_ */

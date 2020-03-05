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
#include "gpio.hpp"

#endif

void setPin();
void setInput();
void setOutput();
void SET(bool check);
void set_Ext_LED(int led_num, bool status);
void init_Ext_LED_as_output(void);

#ifdef __cplusplus
}
#endif

#endif /* C_GPIO_H_ */

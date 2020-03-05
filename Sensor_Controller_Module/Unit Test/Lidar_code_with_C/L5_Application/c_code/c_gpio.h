/*
 * c_gpio.h
 *
 *  Created on: Apr 25, 2019
 *      Author: User
 */

#ifndef C_GPIO_H_
#define C_GPIO_H_

#include <stdbool.h>
#include <stdint.h>

#ifdef __cplusplus
extern "C" {
#endif

void setPin();
void setInput();
void setOutput();
void SET(bool check);


#ifdef __cplusplus
}
#endif


#endif /* C_GPIO_H_ */

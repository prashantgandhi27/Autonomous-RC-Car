/*
 * c_switch.h
 *
 *  Created on: Mar 10, 2019
 *      Author: Vidushi
 */

#ifndef C_SWITCH_H_
#define C_SWITCH_H_

#ifdef __cplusplus
extern "C" {
#endif

#include <stdint.h>
#include <stdbool.h>

bool c_switch_init();
bool c_getSwitch(int num);

#ifdef __cplusplus
}
#endif
#endif /* C_SWITCH_H_ */

/*
 * c_periodic_callbacks.h
 *
 *  Created on: May 3, 2019
 *      Author: Aakash
 */

#ifndef C_PERIODIC_CALLBACKS_H_
#define C_PERIODIC_CALLBACKS_H_

#include "stdint.h"
#include "stdbool.h"

#ifdef __cplusplus
extern "C" {
#endif

bool c_period_init(void);
bool c_period_reg_tlm(void);

void c_period_1Hz(uint32_t count);
void c_period_10Hz(uint32_t count);
void c_period_100Hz(uint32_t count);
void c_period_1000Hz(uint32_t count);

#ifdef __cplusplus
}
#endif

#endif /* C_PERIODIC_CALLBACKS_H_ */

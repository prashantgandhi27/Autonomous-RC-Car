/*
 * c_can.h
 *
 *  Created on: Mar 3, 2019
 *      Author: Aakash
 */

#ifndef C_CAN_H_
#define C_CAN_H_

#include "can.h"
#include "c_io.h"
#include "speed_calculator.h"
#include "generated_can.h"

#ifdef __cplusplus
extern "C" {
#endif


bool motor_can_init(void);
//bool motor_can_rx(CAR_CONTROL_t *drive);
bool motor_can_tx_heartbeat(void);
bool motor_can_reset_busoff(void);
bool send_rpm(void);

#ifdef __cplusplus
}
#endif

#endif /* C_CAN_H_ */

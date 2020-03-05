/*
 * externs.h
 *
 *  Created on: Apr 17, 2019
 *      Author: Aakash
 */

#ifndef EXTERNS_H_
#define EXTERNS_H_
#include "stdbool.h"
#include "../../_can_dbc/generated_can.h"
#include "can.h"

#ifdef __cplusplus
extern "C" {
#endif

extern bool start_cmd;
extern bool sensor_hbt_sync;
extern bool motor_hbt_sync;
extern bool gps_hbt_sync;
extern bool bridge_hbt_sync;


#ifdef __cplusplus
}
#endif

#endif /* EXTERNS_H_ */

/*
 * c_io.h
 *
 *  Created on: Apr 25, 2019
 *      Author: User
 */

#ifndef C_IO_H_
#define C_IO_H_

#include <stdbool.h>
#include <stdint.h>

#ifdef __cplusplus
extern "C" {
#endif

void setLED(uint8_t ledNum, bool on);
void dispDataOnLedDisplay(uint8_t byte_to_display);
void dispDataSetRightDigit(uint8_t alpha);
void dispDataSetLeftDigit(uint8_t alpha);
uint8_t getLSDData(void);

#ifdef __cplusplus
}
#endif


#endif /* C_IO_H_ */

/*
 * c_io.h
 *
 *  Created on: Mar 4, 2019
 *      Author: Aakash
 */

#ifndef C_IO_H_
#define C_IO_H_

#include <stdbool.h>
#include <stdint.h>

#ifdef __cplusplus
extern "C" {
#endif

void clearLedDisplay(void);
void setLED(uint8_t ledNum, bool on);
void dispDataOnLedDisplay(uint8_t byte_to_display);
void dispDataSetRightDigit(uint8_t alpha);
void dispDataSetLeftDigit(char alpha);
uint8_t getLSDData(void);
bool getSwitch(int num);
void init_Ext_LED_as_output(void);
void set_Ext_LED(int led_num, bool status);

#ifdef __cplusplus
}
#endif


#endif /* C_IO_H_ */

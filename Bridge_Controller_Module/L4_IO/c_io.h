/*
 * c_io.h
 *
 *  Created on: Mar 4, 2019
 *      Author: vidushi
 */

#ifndef C_IO_H_
#define C_IO_H_

#include <stdbool.h>
#include <stdint.h>

#ifdef __cplusplus
extern "C" {
#endif


void setLED(uint8_t ledNum, bool on);
void setLCD_Display(char num);
void Clear_Display(void);
void setLCD_LEFT(char num);
void setLCD_Right(char num);
uint8_t switch_detect(void);
bool switch_function(int number);
void init_gpio(void);
void setLED_gpio(int led_num,bool on);

#ifdef __cplusplus
}
#endif


#endif /* C_IO_H_ */

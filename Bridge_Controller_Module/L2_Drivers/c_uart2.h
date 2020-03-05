/*
 * c_uart2.h
 *
 *  Created on: Mar 31, 2019
 *      Author: Vidushi
 */

#ifndef C_UART2_H_
#define C_UART2_H_

#include <stdbool.h>

#ifdef __cplusplus
extern "C" {
#endif

bool cUart2_init(unsigned int baudRate, int rxQSize, int txQSize);

#ifdef __cplusplus
}
#endif

#endif /* C_UART2_H_ */

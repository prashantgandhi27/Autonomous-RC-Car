/*
 * c_wrapper.h
 *
 *  Created on: Apr 29, 2019
 *      Author: Akshata Kulkarni
 */

#ifndef C_WRAPPER_H_
#define C_WRAPPER_H_

#include <stdio.h>
#include "c_uart2.h"
#include "c_uart_dev.h"

#define UART2_BAUD_RATE  38400  ///< Baud rate you wish to use (it will auto-detect and change to this)
#define UART2_RXQ_SIZE   512     ///< Size of UART's RXQ
#define UART2_TXQ_SIZE   10     ///< Size of UART's TXQ
#define FIRMWARE_VERSION    "1.00"

void wUart2_init(void);
bool wgetChar(char* pInputChar, unsigned int timeout);
bool wputChar(char out, unsigned int timeout);
void wputline(char* pBuff, unsigned int timeout);


#endif /* C_WRAPPER_H_ */

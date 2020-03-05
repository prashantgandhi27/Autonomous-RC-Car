/*
 * c_wrapper.c
 *
 *  Created on: Apr 29, 2019
 *      Author: Akshata Kulkarni
 */
#include "c_wrapper.h"

void wUart2_init(void)
{
    cUart2_init(UART2_BAUD_RATE, UART2_RXQ_SIZE, UART2_TXQ_SIZE);
}
bool wgetChar(char* pInputChar, unsigned int timeout)
{
    return(cgetChar(pInputChar,timeout));
}

bool wputChar(char out, unsigned int timeout)
{
    return(cputChar(out,timeout));
}

void wputline(char *pBuff, unsigned int timeout)
{
    cputline(pBuff,timeout);
}

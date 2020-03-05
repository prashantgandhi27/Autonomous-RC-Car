/*
 * c_uart_dev.cpp
 *
 *  Created on: Feb 18, 2019
 *      Author: Vidushi
 */

// @file uart_wrapper.cpp
#include "uart_dev.hpp"
#include "c_uart_dev.h"
#include "char_dev.hpp"
#include "uart2.hpp"
#include <stdbool.h>

extern "C"
{
    bool cgetChar(char* pInputChar, unsigned int timeout)
    {
        return(Uart2::getInstance().getChar(pInputChar, timeout));
    }

    bool cputChar(char out, unsigned int timeout)
    {
        return(Uart2::getInstance().putChar(out, timeout));
    }

    void cputline(char *pBuff, unsigned int timeout)
    {
       Uart2::getInstance().putline(pBuff,timeout);
    }

    bool cgets(char* pBuff, int maxLen, unsigned int timeout)
    {
       return( Uart2::getInstance().gets(pBuff,maxLen,timeout));
    }
}

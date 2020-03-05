/*
 * c_uart2.c
 *
 *  Created on: Mar 31, 2019
 *      Author: Vidushi
 */

#include "uart2.hpp"
#include "c_uart2.h"
#include <stdbool.h>

/**
 * IRQ Handler needs to be enclosed in extern "C" because this is C++ file, and
 * we don't want C++ to "mangle" our function name.
 * This ISR Function need needs to be named precisely to override "WEAK" ISR
 * handler defined at startup.cpp
 */
extern "C"
{
    bool cUart2_init(unsigned int baudRate, int rxQSize, int txQSize)
    {
       return Uart2::getInstance().init(baudRate,  rxQSize,  txQSize);
    }
}

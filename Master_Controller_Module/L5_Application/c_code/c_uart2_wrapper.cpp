/*
 * c_uart2_wrapper.cpp
 *
 *  Created on: Feb 15, 2019
 *      Author: Aakash Chitroda
 *      SJSU ID: 013755040
 */
#include "c_uart2_wrapper.h"
#include <stdio.h>
#include "uart2.hpp"
#include "printf_lib.h"

bool uart2_init(unsigned int baud_rate, int rx_queue_size, int tx_queue_size)
{
    return (Uart2::getInstance().init(baud_rate, rx_queue_size, tx_queue_size));
}

bool uart2_put(const char send_data[], unsigned int timeout_ms)
{
    return (Uart2::getInstance().put(send_data, timeout_ms));
}

bool uart2_put_char(const char send_data, unsigned int timeout_ms)
{
    return (Uart2::getInstance().putChar(send_data, timeout_ms));
}

bool uart2_get(char *recvd_data, int max_length, unsigned int timeout_ms)
{
    return (Uart2::getInstance().gets(recvd_data, max_length, timeout_ms));
}

bool uart2_get_char(char *recvd_data, unsigned int timeout_ms)
{
    return (Uart2::getInstance().getChar(recvd_data, timeout_ms));
}



/*
 * c_uart3_wrapper.h
 *
 *  Created on: Feb 15, 2019
 *      Author: Aakash Chitroda
 *      SJSU ID: 013755040
 */

#ifndef UART3_WRAPPER_H_
#define UART3_WRAPPER_H_
#include "stdint.h"
#include "stdbool.h"

#ifdef __cplusplus
extern "C" {
#endif

bool uart3_init(unsigned int baud_rate, int rx_queue_size, int tx_queue_size);
bool uart3_put(const char send_data[], unsigned int timeout_ms);
bool uart3_put_char(const char send_data, unsigned int timeout_ms);
bool uart3_get(char *recvd_data, int max_length, unsigned int timeout_ms);
bool uart3_get_char(char *recvd_data, unsigned int timeout_ms);

#ifdef __cplusplus
}
#endif


#endif /* UART3_WRAPPER_H_ */

#ifndef C_UART_DEV_H_
#define C_UART_DEV_H_

#include <stdbool.h>

#ifdef __cplusplus
extern "C" {
#endif

bool cgetChar(char* pInputChar, unsigned int timeout);
bool cputChar(char out, unsigned int timeout);
void cputline(const char* pBuff, unsigned int timeout);
bool cgets(char* pBuff, int maxLen, unsigned int timeout);


#ifdef __cplusplus
}
#endif

#endif /* C_UART_DEV_H_ */

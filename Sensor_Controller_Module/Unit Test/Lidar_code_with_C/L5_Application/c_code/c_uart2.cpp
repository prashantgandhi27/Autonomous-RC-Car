#include "c_uart2.h"
#include "uart2.hpp"

// Assuming UART2 is already initialized
// You may want to also add uart2_init() in C

Uart2 *u;

void uart2_instance()
{
 u =  &Uart2::getInstance();
}

void uart2_init(unsigned int baudRate, int rxQSize, int txQSize)
{
  u->init(baudRate,rxQSize,txQSize);
  return;
}

bool uart2_get_char(char* pInputChar,uint32_t timeout_ms) {
  return u->getChar(pInputChar,timeout_ms);
}

bool uart2_put_char(char out, uint32_t timeout_ms)
{
    return u->putChar(out,timeout_ms);
}

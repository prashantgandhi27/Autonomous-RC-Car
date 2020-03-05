#ifndef C_I2C_BASE_H_
#define C_I2C_BASE_H_

#include <stdbool.h>

#ifdef __cplusplus
extern "C" {
#endif

bool I2C_ReadRegisters(uint8_t deviceAddress, uint8_t firstReg, uint8_t* pData, uint32_t bytesToRead);
bool I2C_writeRegisters(uint8_t deviceAddress, uint8_t firstReg, uint8_t* pData, uint32_t bytesToWrite);


#ifdef __cplusplus
}
#endif


#endif /* C_I2C_BASE_H_ */

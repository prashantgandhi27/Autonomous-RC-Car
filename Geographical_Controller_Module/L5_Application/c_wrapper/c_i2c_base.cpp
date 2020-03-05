#include <stdbool.h>
#include "i2c_base.hpp"
#include "i2c2.hpp"
#include "c_i2c_base.h"


extern "C"
{
    bool I2C_ReadRegisters(uint8_t deviceAddress, uint8_t firstReg, uint8_t* pData, uint32_t bytesToRead)
    {
        return(I2C2::getInstance().readRegisters(deviceAddress,firstReg,pData,bytesToRead));
    }

    bool I2C_writeRegisters(uint8_t deviceAddress, uint8_t firstReg, uint8_t* pData, uint32_t bytesToWrite)
    {
        return(I2C2::getInstance().writeRegisters(deviceAddress, firstReg, pData, bytesToWrite));
    }
}

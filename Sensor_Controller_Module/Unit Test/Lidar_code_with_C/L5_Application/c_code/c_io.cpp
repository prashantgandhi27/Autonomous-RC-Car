/*
 * c_io.cpp
 *
 *  Created on: Apr 25, 2019
 *      Author: User
 */

#include "c_code/c_io.h"
#include "io.hpp"

void setLED(uint8_t ledNum, bool on)
{
    LE.set(ledNum, on);
}

void dispDataOnLedDisplay(uint8_t byte_to_display)
{
    LD.setNumber(byte_to_display);
}

void dispDataSetRightDigit(uint8_t alpha)
{
    LD.setRightDigit(alpha);
}

void dispDataSetLeftDigit(uint8_t alpha)
{
    LD.setLeftDigit(alpha);
}

uint8_t getLSDData(void)
{
    return (LS.getPercentValue());
}


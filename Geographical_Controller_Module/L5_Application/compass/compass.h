#ifndef COMPASS_H_
#define COMPASS_H_

#ifdef __cplusplus
extern "C" {
#endif
#include <stdint.h>

#define COMPASS_I2C_ADDR    0xC0                   //I2C Address of Compass
#define COMMAND_REG_ADDR    0x00                   //Address of command register of compass
#define COMPASS_BEARING_16BIT_REG_ADDR 0x02     //Address of register containing compass bearing
#define READ_ONE_REGISTER   1                     //Read one register
#define READ_TWO_REGISTERS  2                    //Read two registers
#define COMPASS_OFFSET 10;

enum {
    sw1 = (1 << 0),
    sw2 = (1 << 1),
    sw3 = (1 << 2),
    sw4 = (1 << 3),
}switch_pressed;

float Compass_Get_Bearing_Angle(void);
void Start_Compass_Calibration(void);
void Stop_Compass_Calibration(void);
void Set_Default_Calibration(void);
void get_compass_data(void);

#ifdef __cplusplus
}
#endif

#endif /* COMPASS_H_ */

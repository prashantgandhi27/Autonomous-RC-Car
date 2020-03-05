#pragma once

#include "rptypes.h"
#include "rplidar_cmd.h"
#include "uart2.hpp"

typedef struct
{
    float distance;
    float angle;
    uint8_t quality;
    bool  startBit;
}RPLidarMeasurement;

typedef struct
{
    int s0;
    int s1;
    int s2;
    int s3;
    int s4;
    int s5;
    int s6;
    int s7;
    int s8;
    int s9;
    int s10;
    int s11;
}RPLidarRotation;

class RPLidar
{
    public:
        enum{
            RPLIDAR_SERIAL_BAUDRATE = 115200,
            RPLIDAR_DEFAULT_TIMEOUT = 500,
        };

    Uart2 &uart2 = Uart2::getInstance();
   // bool uart2_init(unsigned int baudRate, int rxQSize=32, int txQSize=64);
    // ask the RPLIDAR for its health info
    uint32_t getHealth(rplidar_response_device_health_t & healthinfo, uint32_t timeout = RPLIDAR_DEFAULT_TIMEOUT);

    // ask the RPLIDAR for its device info like the serial number
    uint32_t getDeviceInfo(rplidar_response_device_info_t & info, uint32_t timeout = RPLIDAR_DEFAULT_TIMEOUT);

    // stop the measurement operation
    u_result stop();

    uint32_t reset();
    // start the measurement operation
    uint32_t startScan(bool force = false, uint32_t timeout = RPLIDAR_DEFAULT_TIMEOUT*2);

    // wait for one sample point to arrive
    uint32_t waitPoint(rplidar_response_measurement_node_t *node, uint32_t timeout, RPLidarMeasurement *measurement);

    // retrieve currently received sample point

    const RPLidarMeasurement & getCurrentPoint()
    {
        return _currentMeasurement;
    }

    int divideDistance(float distance);
    int divideDistanceback(float distance);
    void divideAngle(RPLidarMeasurement *angle_value,int length, RPLidarRotation *rot);
    void set_LEDS();
protected:
    uint32_t _sendCommand(uint8_t cmd, const void * payload, size_t payloadsize);
    uint32_t _waitResponseHeader(rplidar_ans_header_t * header, uint32_t timeout);

protected:
    RPLidarMeasurement _currentMeasurement;
};

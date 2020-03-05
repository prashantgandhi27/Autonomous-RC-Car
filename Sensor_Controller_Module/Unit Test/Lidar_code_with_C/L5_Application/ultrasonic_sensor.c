/*
 * ultrasonic_sensor.c
 *
 *  Created on: Apr 25, 2019
 *      Author: User
 */

#include <stdint.h>
#include <stdbool.h>
#include "ultrasonic_sensor.h"
#include "eint.h"
#include "c_code/c_gpio.h"
#include "lpc_sys.h"
#include "printf_lib.h"
#include "utilities.h"
#include "c_code/c_io.h"

sensor_lv_max_sonar_t send_ultrasonic_data;

static void on_pw_rising_edge(void)
{
    send_ultrasonic_data.start_echo_time = sys_get_uptime_us();
}

static void on_pw_falling_edge(void)
{
    send_ultrasonic_data.end_echo_time = sys_get_uptime_us();
    send_ultrasonic_data.distance = (double)(send_ultrasonic_data.end_echo_time - send_ultrasonic_data.start_echo_time)*(1.0/58.0);
}

void sensor_cntlr_init(void)
{
    setPin();
    setInput();

    eint3_enable_port2(1, eint_rising_edge, on_pw_rising_edge);
    eint3_enable_port2(1, eint_falling_edge, on_pw_falling_edge);

    sensor_start_scan();
}

void sensor_start_scan(void)
{
    SET(0);
    delay_us(5);
    SET(1);
    delay_us(25);
}

void sensor_stop_scan(void)
{
    SET(0);
}


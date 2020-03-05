/**
 * @file
 *
 * The purpose of this "C" callbacks is to provide the code to be able
 * to call pure C functions and unit-test it in C test framework
 */
#include <can_receive.h>
#include <stdint.h>
#include <stdbool.h>
#include "c_pwm.h"
#include "c_period_callbacks.h"
#include "motor_can_rx.h"
#include "motor_control.h"
#include "speed_calculator.h"
#include "eint.h"
#include "c_io.h"
#include "c_gpio.h"
#include "lcd.h"
#include "lpc_timers.h"
#include "printf_lib.h"
#include "rpm/c_pid.h"

extern s_pid_t dc;
double pwm_val_dc = 15.00;

bool C_period_init(void) {
    setLED(1, 0);
    motor_can_init();
    init_pwm(100);
    set_pwm_value(motor_1, 15);
    set_pwm_value(servo_2, 15);
    GPIO_interrupt();
    eint3_enable_port2(5, eint_falling_edge, eint3_handler);
    lcd_init();
    createPID(&dc);
    init_LEDs();
    return true;
}
bool C_period_reg_tlm(void) {
    return true;
}

void C_period_1Hz(uint32_t count) {
    (void) count;
    motor_can_tx_heartbeat();
    motor_can_reset_busoff();
    send_rpm();
    lcd_screen_query();
}

void C_period_10Hz(uint32_t count) {
    (void) count;
    lcd_receive();
    if (count % 4 == 0)
        calculate_speed();
    command_motor(&drive, &pwm_val_dc);
}

void C_period_100Hz(uint32_t count) {
    (void) count;
    receive_can_msg();
    command_servo(&drive);
}

void C_period_1000Hz(uint32_t count) {
    (void) count;
}


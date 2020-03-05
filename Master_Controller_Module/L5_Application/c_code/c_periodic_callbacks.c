/*
 * c_periodic_callbacks.c
 *
 *  Created on: May 3, 2019
 *      Author: Aakash
 */
#include "c_periodic_callbacks.h"
#include "master.h"
#include "c_io.h"
#include "externs.h"
#include <stdio.h>
#include "defines.h"

extern bool start_free_run_flag;
extern bool free_run_motor_flag;
extern bool free_steer_flag;

bool c_period_init(void)
{
    clearLedDisplay();
    master_controller_init();
    init_Ext_LED_as_output();
    return true;
}

bool c_period_reg_tlm(void)
{
    return true;
}

void c_period_1Hz(uint32_t count)
{
    (void) count;
    master_CAN_turn_on_bus_if_bus_off();
    master_send_debug_msg();
    transmit_heartbeat_on_can();
}

void c_period_10Hz(uint32_t count)
{
    (void) count;
    if (getSwitch(1))
    {
        start_free_run_flag = !start_free_run_flag;
    }

    if (getSwitch(2))
    {
        free_run_motor_flag = !free_run_motor_flag;
        if (free_run_motor_flag)
        {
            set_drive_cmd(MOTOR_FORWARD);
            set_drive_speed(MOTOR_SLOW_KPH);
        }
        else
        {
            set_drive_cmd(MOTOR_STOP);
            set_drive_speed(MOTOR_STOP_KPH);
        }
    }

    if (getSwitch(3))
    {
        set_drive_cmd(MOTOR_REV);
        set_drive_speed(MOTOR_SLOW_KPH);
    }

    if (getSwitch(4))
    {
        free_steer_flag = !free_steer_flag;
        if (free_steer_flag)
            set_steer_cmd(FULL_RIGHT);
        else
            set_steer_cmd(FULL_LEFT);
    }
}

void c_period_100Hz(uint32_t count)
{
    (void) count;
    master_service_can_msgs_at_100Hz();
    display_hbt_sync_from_all_controllers();
    if (start_free_run_flag == false)
    {
        if ((true == hbt_sync_from_all_node()) && (true == sys_start_stop_cmd()))
        {
            if(start_obstacle_avoidance())
            {
                dispDataSetLeftDigit('1');
            }
            else
            {
                navigate_car_to_destination();
                dispDataSetLeftDigit('0');
            }
        }
        else
        {
            set_drive_cmd(MOTOR_STOP);
            set_drive_speed(MOTOR_STOP_KPH);
            set_steer_cmd(STRAIGHT);
        }
    }
    master_send_command_to_motor_module();
}

void c_period_1000Hz(uint32_t count)
{
    (void) count;
}



/*
 * steer_processor.c
 *
 *  Created on: Feb 24, 2019
 *      Author: halak
 */
#include <stdint.h>
#include "steer_processor.h"

void steer_processor(int32_t left_sensor_cm, int32_t right_sensor_cm)
{
   if(left_sensor_cm > 50 && right_sensor_cm < 50)
   {
     steer_left();
   }
   else if(right_sensor_cm > 50 && left_sensor_cm < 50)
   {
       steer_right();
   }
   else if(left_sensor_cm <= 50 && right_sensor_cm <= 50)
   {
       if(left_sensor_cm >= right_sensor_cm)
       {
           steer_left();
       }
       else
       {
           steer_right();
       }
   }
}

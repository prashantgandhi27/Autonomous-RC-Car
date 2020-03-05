/*
 * BLE_HC05.c
 *
 *  Created on: Mar 31, 2019
 *      Author: Vidushi
 *      Version: V1.00
 */

#include <stdio.h>
#include <stdlib.h>
#include "../Bridge_Module/ble_hc05.h"

ble_msg_t ble_param = {0};

void BLE_init(void)
{
    Clear_Display();      /* Clear the display */
    wUart2_init();

    printf("\r\nHC-05 Version:%s\r\n",FIRMWARE_VERSION);
    // Default Values for now
    //ble_param.latitude = "Lat:37.121356 ";
    //ble_param.longitude = "Long:-121.121356 ";
    //ble_param.getdata = "Lat:37.121356";

}

void BLE_rx(void)
{
   char sLat[] = "Lat:";
   char sLong[] = "Long:";
   char start[] = "S1";
   char stop[] = "S0";
   static uint8_t count =0;
   uint8_t cpFlag = 0;
   char* temp;

  while(cgets(ble_param.getdata, 120, 10))
  {
      printf("Data from App: %s",ble_param.getdata);
  }

      if(!strcmp(ble_param.getdata,start))
      {
          count++;
          printf("count:%d\n",count);
          CAN_Transmit_start(START_CMD);
      }
      if(!strcmp(ble_param.getdata,stop))
      {
          CAN_Transmit_start(STOP_CMD);
      }

      if(strstr(ble_param.getdata,sLat)!= NULL)
      {
          temp = find_list_element(ble_param.getdata,1,':');
          checkpoint.CHECKPOINT_LAT_deg = atof(temp);
          printf("checkpoint Latitude:%f",checkpoint.CHECKPOINT_LAT_deg);
          cpFlag = 1;
      }
      if(strstr(ble_param.getdata,sLong)!= NULL)
      {
           temp = find_list_element(ble_param.getdata,1,':');
           checkpoint.CHECKPOINT_LONG_deg = atof(temp);
           printf("checkpoint Longitude:%f",checkpoint.CHECKPOINT_LONG_deg);
           cpFlag = 2;
      }
    if(cpFlag == 2)
    {
        printf("Flag: %d",cpFlag);
        CAN_Transmit_gpsCheckpoint();
        cpFlag = 0;
    }
  }

void BLE_tx(void)
{
    wputline(ble_param.bearing,10);
    wputline(ble_param.MotorDriveCmd,10);
    wputline(ble_param.MotorSteerCmd,10);
    wputline(ble_param.RPM,10);
    wputline(ble_param.obstacleFront,10);
    wputline(ble_param.obstacleRight,10);
    wputline(ble_param.obstacleLeft,10);
    wputline(ble_param.obstacleBack,10);
    wputline(ble_param.heading,10);
    wputline(ble_param.distance,10);
    if(ble_param.latitude != '\0' && ble_param.longitude != '\0')
    {
    wputline(ble_param.latitude,10); //Adjust Delay as per data length
    wputline(ble_param.longitude,10);
    }
}

char* find_list_element(char* list_str, int index, char separator)
{
    /* Sanity check. */
    if (list_str == 0 || index < 0) {
        return "";
    }

    int curr_index = 0;

    /* Find the element. */
    while (curr_index < index && *list_str != 0) {
        if (*list_str++ == separator) {
            ++curr_index;
        }
    }

    /* The element was not found? */
    if (index != curr_index || *list_str == 0) {
        return "";
    }

    return list_str;
}


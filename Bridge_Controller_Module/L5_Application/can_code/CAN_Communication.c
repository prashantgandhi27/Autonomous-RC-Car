
#include <stdio.h>
#include "CAN_Communication.h"

BRIDGE_NODE_t start_cmd = {0}; // 103
//--------------------------------------
CAR_CONTROL_t MotorCMD = {0}; //104
SENSOR_NODE_t obstacle = {0}; // 105
MOTOR_NODE_t MotorRPM = {0};  // 106
//-------------------------------------------
GPS_LOCATION_t RX_data ={0}; // 108
COMPASS_t compass_data = {0}; // 109
MASTER_HEARTBEAT_t heartbeat = {0}; //110
BRIDGE_DEBUG_t debug = {0};  //119

const uint32_t GPS_LOCATION__MIA_MS = 3000;
const GPS_LOCATION_t GPS_LOCATION__MIA_MSG = { 0 };
const uint32_t COMPASS__MIA_MS=3000;
const COMPASS_t COMPASS__MIA_MSG={0};
const uint32_t  MASTER_HEARTBEAT__MIA_MS=3000;
const MASTER_HEARTBEAT_t MASTER_HEARTBEAT__MIA_MSG={0};

ble_msg_t ble_param;
static bool setHeartbeat;

#define MAX_LOCATIONS   7//20
#define MASTER_HEARTBEAT_MSG_ID 110

void CAN_Init_w(void)
{
    Clear_Display();
    if(CAN_init(can1, 100, 100, 100, NULL, NULL))
    {
        CAN_bypass_filter_accept_all_msgs();
        CAN_reset_bus(can1);
        start_cmd.BRIDGE_START_cmd = 0x0;
        debug.IO_DEBUG_CAN_init =0x00;
    }
    else
        debug.IO_DEBUG_CAN_init =0x01;

}

void CANresetAfterBusOff(void)
{
    if(CAN_is_bus_off(can1))
    {
        debug.IO_DEBUG_bus_off = 0x01;
        printf("can bus off\n");
        //CAN_bypass_filter_accept_all_msgs();
        CAN_reset_bus(can1);

    }
    else
        debug.IO_DEBUG_bus_off = 0x00;
}

void CAN_Recieve(void)
{
    can_msg_t can_msg;
    while(CAN_rx(can1, &can_msg, 0))
    {
        dbc_msg_hdr_t can_msg_hdr;
        can_msg_hdr.dlc = can_msg.frame_fields.data_len;
        can_msg_hdr.mid = can_msg.msg_id;

        if(can_msg_hdr.mid == 104)
        {
            dbc_decode_CAR_CONTROL(&MotorCMD,can_msg.data.bytes, &can_msg_hdr);
            sprintf(ble_param.MotorDriveCmd," DrvCmd:%d",MotorCMD.MOTOR_DRIVE_cmd);
            sprintf(ble_param.MotorSteerCmd," StrCmd:%f",MotorCMD.MOTOR_STEER_cmd);
        }

        else if(can_msg_hdr.mid == 105)
       {
           dbc_decode_SENSOR_NODE(&obstacle,can_msg.data.bytes, &can_msg_hdr);
           sprintf(ble_param.obstacleFront, " ObsFront:%u",obstacle.SENSOR_FRONT_cm);
           sprintf(ble_param.obstacleRight," ObsRight:%u",obstacle.LIDAR_Obstacle_RIGHT);
           sprintf(ble_param.obstacleLeft," ObsLeft:%u",obstacle.LIDAR_Obstacle_LEFT);
           sprintf(ble_param.obstacleBack," ObsBack:%u",obstacle.LIDAR_Obstacle_BACK);
       }

        else if (can_msg_hdr.mid == 106)
        {
            dbc_decode_MOTOR_NODE(&MotorRPM,can_msg.data.bytes, &can_msg_hdr);
            sprintf(ble_param.RPM, "RPM:%f", MotorRPM.MOTOR_SPEED_kph);
        }

        else if(can_msg_hdr.mid == 108)
        {
            setLED(1,1);
            dbc_decode_GPS_LOCATION(&RX_data, can_msg.data.bytes, &can_msg_hdr);
            sprintf(ble_param.latitude, " Lat:%f", RX_data.CURRENT_LAT_deg);
            sprintf(ble_param.longitude," Long:%f", RX_data.CURRENT_LONG_deg);
            debug.IO_DEBUG_Bridge_rx = 0x00;
        }
        else if(can_msg_hdr.mid == 109)
        {
            setLED(2,1);
            dbc_decode_COMPASS(&compass_data, can_msg.data.bytes, &can_msg_hdr);
            sprintf(ble_param.bearing, " Bearing:%f", compass_data.CMP_BEARING_deg);
            sprintf(ble_param.heading, " Heading:%f", compass_data.CMP_HEADING_deg);
            sprintf(ble_param.distance, " Distance:%f", compass_data.CMP_DISTANCE_meters);
            debug.IO_DEBUG_Bridge_rx = 0x00;
        }
        else if(can_msg_hdr.mid == 110)
        {
              dbc_decode_MASTER_HEARTBEAT(&heartbeat, can_msg.data.bytes, &can_msg_hdr);
              if(heartbeat.MASTER_hbt == 1)
              {
                  setHeartbeat = true;
                  setLED_gpio(2,true);
              }
        }
    }

  if(dbc_handle_mia_GPS_LOCATION(&RX_data, 100))
  {
      //setLED(1,0);
      //printf("Mia Occured:GPS");
      debug.IO_DEBUG_Bridge_rx = 0x01;
  }

  if(dbc_handle_mia_COMPASS(&compass_data,100))
  {
      //setLED(2,0);
      //printf("Mia Occured:Compass");
      debug.IO_DEBUG_Bridge_rx = 0x01;
  }
  if(dbc_handle_mia_MASTER_HEARTBEAT(&heartbeat,25))
  {
      //printf("Mia Occured:Compass");
      setHeartbeat = false;
      setLED_gpio(2,false);
      debug.IO_DEBUG_Bridge_rx = 0x01;
  }
}


void CAN_Transmit_start(uint8_t data)
{
    printf("Transmit : %d\n",data);
    bool success;
    start_cmd.BRIDGE_START_cmd = data;
   can_msg_t can_msg = { 0 };

   // Encode the CAN message's data bytes, get its header and set the CAN message's DLC and length
   dbc_msg_hdr_t msg_hdr = dbc_encode_BRIDGE_NODE(can_msg.data.bytes, &start_cmd);
   can_msg.msg_id = msg_hdr.mid;
   can_msg.frame_fields.data_len = msg_hdr.dlc;
   // Queue the CAN message to be sent out
   data?setLCD_Right('1'):setLCD_Right('0');
   data?setLED_gpio(1,true):setLED_gpio(1,false);
   success = CAN_tx(can1, &can_msg, 0);
   //printf("Transmit : %d\n",data);
}

void CAN_Transmit_gpsCheckpoint(void)
{
   bool success;
    can_msg_t can_msg = { 0 };
    //checkpoint.CHECKPOINT_LAT_deg = 37.121356;
    //checkpoint.CHECKPOINT_LONG_deg =-121.121356;
   // Encode the CAN message's data bytes, get its header and set the CAN message's DLC and length
   dbc_msg_hdr_t msg_hdr = dbc_encode_BRIDGE_CHECKPOINTS(can_msg.data.bytes, &checkpoint);
   can_msg.msg_id = msg_hdr.mid;
   can_msg.frame_fields.data_len = msg_hdr.dlc;
   // Queue the CAN message to be sent out
   success = CAN_tx(can1, &can_msg, 0);
  // printf("Transmit : %d\n",success);
}

bool transmit_heartbeat_on_can(void)
{
   BRIDGE_HEARTBEAT_t heartbeat_msg = {0};
   can_msg_t can_msg = { 0 };

   heartbeat_msg.BRIDGE_hbt = 1;

   dbc_msg_hdr_t msg_hdr = dbc_encode_BRIDGE_HEARTBEAT(can_msg.data.bytes, &heartbeat_msg);
   can_msg.msg_id = msg_hdr.mid;
   can_msg.frame_fields.data_len = msg_hdr.dlc;


   // Queue the CAN message to be sent out
   if(CAN_tx(can1, &can_msg, 0))
   {
      debug.IO_DEBUG_HBT_Transmit = 0x00;
      return true;
   }
   else
   {
       debug.IO_DEBUG_HBT_Transmit = 0x01;
       return false;
   }

}

bool transmit_debug_on_can(void)
{
    can_msg_t can_msg = { 0 };
    dbc_msg_hdr_t msg_hdr = dbc_encode_BRIDGE_DEBUG(can_msg.data.bytes, &debug);
    can_msg.msg_id = msg_hdr.mid;
    can_msg.frame_fields.data_len = msg_hdr.dlc;
    return(CAN_tx(can1, &can_msg, 0));
}





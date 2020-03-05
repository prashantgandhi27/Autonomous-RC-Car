#include "can.h"
#include <stdio.h>
#include <stdbool.h>
#include "generated_can.h"
#include "can_tx.h"
#include "gps.h"
#include "c_io.h"
#include "compass.h"

can_msg_t msg;

#define CAN_BAUDRATE    100
#define TX_SIZE_DATA    100
#define RX_SIZE_DATA    100

GPS_DEBUG_t debug_cmd = {0};
static bool setHeartbeat = false;
uint8_t location_index = MAX_ROUTE_INDEX;
uint8_t Prev_index = MAX_ROUTE_INDEX;

double calculated_distance = 0.0;
//double checkpoint_lat = 37.335200,checkpoint_long =  -121.888700;

double checkpoint_lat, checkpoint_long;
BRIDGE_CHECKPOINTS_t startLoc = {0};
MASTER_HEARTBEAT_t heartbeat = {0};
const uint32_t  MASTER_HEARTBEAT__MIA_MS=3000;
const MASTER_HEARTBEAT_t MASTER_HEARTBEAT__MIA_MSG={0};

bool can_init(void)
{
    bool val;

    if(CAN_init(can1, CAN_BAUDRATE, RX_SIZE_DATA, TX_SIZE_DATA, NULL, NULL)) {
        CAN_bypass_filter_accept_all_msgs();
        CAN_reset_bus(can1);
        val=true;
    }
    else {
        val = false;
    }

    debug_cmd.IO_DEBUG_CAN_init = (char)val;
    return val;
}

bool transmit_gps_data_on_can(void)
{
   // if(setHeartbeat)
    {
        GPS_LOCATION_t gps_cmd = {0};
        gps_cmd.CURRENT_LAT_deg = (float)getLatitude();
        gps_cmd.CURRENT_LONG_deg = (float)getLongitude();

        if(gps_cmd.CURRENT_LAT_deg == INVALID_COORDINATES) {
            debug_cmd.IO_DEBUG_GPS_Fix = 1;
            debug_cmd.IO_DEBUG_GPS_rx = 1;
        }
        else {
            debug_cmd.IO_DEBUG_GPS_Fix = 0;
            debug_cmd.IO_DEBUG_GPS_rx = 0;
        }

        can_msg_t can_msg = { 0 };

        // Encode the CAN message's data bytes, get its header and set the CAN message's DLC and length
        dbc_msg_hdr_t msg_hdr = dbc_encode_GPS_LOCATION(can_msg.data.bytes, &gps_cmd);
        can_msg.msg_id = msg_hdr.mid;
        can_msg.frame_fields.data_len = msg_hdr.dlc;

        // Queue the CAN message to be sent out
        return (CAN_tx(can1, &can_msg, 0));
    }
}

bool transmit_compass_data_on_can(void)
{
    static int ld_count = 0;
    double Heading_value = 0.0;
  // if(setHeartbeat)
    COMPASS_t compass_msg = {0};
    can_msg_t can_msg = { 0 };

    float compass_Bearing_value = Compass_Get_Bearing_Angle();

    if(compass_Bearing_value == 0.0)
        debug_cmd.IO_DEBUG_Compass_Rx = 1;
    else
        debug_cmd.IO_DEBUG_Compass_Rx = 0;

    if(location_index >= MAX_ROUTE_INDEX || calculated_distance <= 5.0)
    {
       location_index = calculate_initial_checkpoint(latitude,longitude);
    }
    calculated_distance = calculate_target_distance(gps_checkpoints[location_index][0], gps_checkpoints[location_index][1],latitude,longitude); //gps_checkpoints[location_index][0], gps_checkpoints[location_index][1]
    Heading_value = HeadingAngle(gps_checkpoints[location_index][0], gps_checkpoints[location_index][1]);

    compass_msg.CMP_BEARING_deg = compass_Bearing_value;
    compass_msg.CMP_HEADING_deg = (float)Heading_value;
    compass_msg.CMP_DISTANCE_meters = (float)calculated_distance;

    ld_count++;
    if((ld_count % 5) == 0) //500ms
    {
        ld_count = 0;
        Clear_Display();
        setLCD_Display(location_index);
    }

    dbc_msg_hdr_t msg_hdr = dbc_encode_COMPASS(can_msg.data.bytes, &compass_msg);
    can_msg.msg_id = msg_hdr.mid;
    can_msg.frame_fields.data_len = msg_hdr.dlc;

    // Queue the CAN message to be sent out
    return (CAN_tx(can1, &can_msg, 0));
}

bool transmit_debug_data_on_can(void){
    can_msg_t can_msg = { 0 };

    // Encode the CAN message's data bytes, get its header and set the CAN message's DLC and length
    dbc_msg_hdr_t msg_hdr = dbc_encode_GPS_DEBUG(can_msg.data.bytes, &debug_cmd);
    can_msg.msg_id = msg_hdr.mid;
    can_msg.frame_fields.data_len = msg_hdr.dlc;

    // Queue the CAN message to be sent out
    return (CAN_tx(can1, &can_msg, 0));
}

bool transmit_heartbeat_on_can(void)
{
    GPS_HEARTBEAT_t heartbeat_msg = {0};
    can_msg_t can_msg = { 0 };

    heartbeat_msg.GPS_hbt = GPS_HEART_BEAT;

    dbc_msg_hdr_t msg_hdr = dbc_encode_GPS_HEARTBEAT(can_msg.data.bytes, &heartbeat_msg);
    can_msg.msg_id = msg_hdr.mid;
    can_msg.frame_fields.data_len = msg_hdr.dlc;

    debug_cmd.IO_DEBUG_HBT_Transmit = (char)CAN_tx(can1, &can_msg, 0);
    // Queue the CAN message to be sent out
    return ((bool)debug_cmd.IO_DEBUG_HBT_Transmit);
}

void can_receive(void)
{
    can_msg_t can_msg;

    while(CAN_rx(can1, &can_msg, 0))
    {
        dbc_msg_hdr_t msg_hdr_receive;
        msg_hdr_receive.mid = can_msg.msg_id;
        msg_hdr_receive.dlc = can_msg.frame_fields.data_len;

        switch(can_msg.msg_id)
        {
            case MASTER_HEARTBEAT_MSG_ID:
               dbc_decode_MASTER_HEARTBEAT(&heartbeat, can_msg.data.bytes, &msg_hdr_receive);
                   if(heartbeat.MASTER_hbt == 1)
                   {
                       setHeartbeat = true;
                       setLED_gpio(2,true);
                   }
               break;

            case BRIDGE_CHECKPOINTS_ID:
                dbc_decode_BRIDGE_CHECKPOINTS(&startLoc, can_msg.data.bytes, &msg_hdr_receive);
                checkpoint_lat = (double)startLoc.CHECKPOINT_LAT_deg;
                checkpoint_long = (double)startLoc.CHECKPOINT_LONG_deg;
                location_index = MAX_ROUTE_INDEX;
                Prev_index = MAX_ROUTE_INDEX;
                break;
        }
    }

    if(dbc_handle_mia_MASTER_HEARTBEAT(&heartbeat,25))
    {
      setHeartbeat = false;
      setLED_gpio(2,false);
      //debug.IO_DEBUG_Bridge_rx = 0x01;
    }

}

void check_bus_off(void){
    if(CAN_is_bus_off(can1)){
        debug_cmd.IO_DEBUG_bus_off = 1;
        CAN_reset_bus(can1);
        CAN_bypass_filter_accept_all_msgs();
    }
    else
        debug_cmd.IO_DEBUG_bus_off = 0;
}

uint8_t calculate_initial_checkpoint(double cur_lat, double cur_long)
{
    if(checkpoint_lat == INVALID_COORDINATES && checkpoint_long == INVALID_COORDINATES)
         return DEFAULT_LOC;
    //car to dest or most recently selected route checkpoint to destination

    setLCD_Display(1);
    bool isThereInitialCheckpoint = false;
    uint8_t checkpoint_index = 0;
    double src_distance_to_dest = calculate_target_distance(cur_lat, cur_long, checkpoint_lat, checkpoint_long);
    double car_distance_to_checkpoint, checkpoint_distance_to_dest;
    double shortest_checkpoint_distance = GPS_CHECKPOINT_MAX_DISTANCE;
    //loop through all our checkpoints and find the closest one to our src that also closes the distance to dest
    for(int i = 0; i < MAX_ROUTE_INDEX; i++)
    {
        //cur_lat and cur_long are current car coords
        car_distance_to_checkpoint = calculate_target_distance(cur_lat, cur_long, gps_checkpoints[i][0], gps_checkpoints[i][1]);
        checkpoint_distance_to_dest = calculate_target_distance(gps_checkpoints[i][0], gps_checkpoints[i][1], checkpoint_lat, checkpoint_long);
        if((checkpoint_distance_to_dest < src_distance_to_dest) && (car_distance_to_checkpoint < shortest_checkpoint_distance) && (src_distance_to_dest > 2.0))
        {
            setLED(1,1);
            if(Prev_index != i)
            {
                shortest_checkpoint_distance = car_distance_to_checkpoint;
                checkpoint_index = i;
                isThereInitialCheckpoint = true;
            }
        }
    }

    //only if there is a checkpoint that satisfies the above conditions can we say there is an initial checkpoint and can add to route_checkpoints
    if(isThereInitialCheckpoint)
    {
        setLED(1,0);
        Prev_index = checkpoint_index;
       // route_index[0] = checkpoint_index;
        return checkpoint_index;
    }
    else
    {
        //returning this magic number means we didn't find a checkpoint, assume it's just a straight line to dest
        return DEFAULT_LOC;
    }
}

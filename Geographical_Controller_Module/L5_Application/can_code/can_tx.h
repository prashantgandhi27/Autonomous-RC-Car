#ifndef CAN_TX_H_
#define CAN_TX_H_

#include <stdint.h>
#include <stdbool.h>

#define GPS_HEART_BEAT 1
#define MASTER_HEARTBEAT_MSG_ID 110
#define BRIDGE_CHECKPOINTS_ID   107
#define MAX_DISPLAY_DISTANCE    100
#define MAX_LOCATIONS           27//33
#define MAX_ROUTE_INDEX     26//32
#define DEFAULT_LOC         MAX_ROUTE_INDEX

bool can_init(void);
//extern void separate_task(void *p);
//void separate_task(void *p);
bool transmit_dbc_data_on_can(void);
void check_bus_off(void);
bool transmit_compass_data_on_can(void);
bool transmit_gps_data_on_can(void);
bool transmit_heartbeat_on_can(void);
bool transmit_debug_data_on_can(void);
void can_receive(void);
uint8_t calculate_initial_checkpoint(double cur_lat, double cur_long);

extern double checkpoint_lat,checkpoint_long, calculated_distance;
extern uint8_t location_index;

const float gps_checkpoints[MAX_LOCATIONS][2] = { //200 something bytes
        {37.338951, -121.880177},  //0
               {37.338878, -121.880331},  //1
               {37.338820, -121.880456},  //2
               {37.338803, -121.880607},  //3
               {37.338973, -121.880234},  //4
               {37.338917, -121.880358},  //5
               {37.338860, -121.880484},  //6
               {37.338925, -121.880698},  //7
               {37.339031,-121.880849},
               {37.339351,-121.881136},
               {37.339146, -121.880863},  //8  //0.42
               {37.339292, -121.880972},  //9
               {37.339524, -121.881147},  //10
               {37.339672, -121.881261},  //11
               {37.339736, -121.881136},  //12
               {37.339513, -121.880964}, //13  //source - dest 0.085
               {37.339208, -121.880735}, //14  //0.04
               {37.339792, -121.881012},  //15
              // {37.339748, -121.880978},//16
               {37.339571, -121.880843}, //16
               {37.339269, -121.880624}, //17
               {37.339846, -121.880886},  //18
          //     {37.339266, -121.880621}, //20
               {37.339325, -121.880495}, //19
               {37.339896, -121.880769}, //20
               {37.339682, -121.880597}, //21
               {37.339382, -121.880371},  //22
               {37.339069, -121.880138},  //23
               {0.0,0.0}, //32
};

#endif /* CAN_TX_H_ */

#include "gps.h"
#include "c_uart2.h"
#include "c_io.h"
#include "c_uart_dev.h"
#include "utilities.h"
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <stdbool.h>
#include <math.h>
#include "FreeRTOS.h"


#define R   6371.0
#define PI  3.14159



void create_gps_config(void)
{
    //cputline(PMTK_SET_BAUD_57600,30);
    cputline(PMTK_SET_NMEA_OUTPUT_RMCONLY, 10);
    cputline(PMTK_SET_NMEA_UPDATE_10HZ, 10);
    cputline(PMTK_API_SET_FIX_CTL_5HZ, 10);
}

//UART INIT
void init_gps(void)
{
    Clear_Display();      /* Clear the display */
    cUart2_init(9600, UART_RECEIVE_BUFFER_SIZE, UART_TRANSMIT_BUFFER_SIZE);      /* Init baud rate */
    create_gps_config();
}

void gps_rx(void)
{

    delay_ms(100);

    if (cgets(gps_line, 120, 100)) {
        char valid_invalid[2]={0};
        char timeStamp[10];
        char south_or_north[2]={0};
        double tempLat;
        double tempLon;
          // $GPRMC,081836,A,3751.65,S,14507.36,E,000.0,360.0,130998,011.3,E*62
         //   if (5 == sscanf(gps_line, "$GPRMC,%d,%c,%lf,%c,%lf", timeStamp, &valid_invalid, &tempLat, &south_or_north, &tempLon)) {
          if (5 == sscanf(gps_line, "$GPRMC,%[^,],%2[^,],%lf,%2[^,],%lf", timeStamp, valid_invalid, &tempLat, south_or_north, &tempLon)) {
              portENTER_CRITICAL();
              setLED_gpio(1,true);
              latitude = (tempLat - 100*(int)(tempLat/100))/60.0;
              latitude += (int)(tempLat/100);
           //   printf("%f,%f",tempLat,tempLon);
              longitude = (tempLon - 100*(int)(tempLon/100))/60.0;
              longitude += (int)(tempLon/100);
              longitude = -longitude;
              portEXIT_CRITICAL();
          }
          else
              setLED_gpio(1,false);

      }
    //printf("%s",gps_line);
}

bool gps_parse_data(void)
{
    if(!memcmp(gps_line, "$GPRMC", 6) && gps_line[18]=='A' && gps_line[30]=='N' && gps_line[43]=='W')
    {
       // setLED(2,1);

        char lat[10] = "";
        char lon[11] = "";

        //3720.1308,-12152.9826 $GPRMC,003755.400,A,3720.1674,N,12152.9132,W,0.01,233.25,120519,,,A*
        memcpy(lat, (gps_line+20), 9);
        memcpy(lon, (gps_line+32), 10);

        double tempLat = ((atof)(lat));        // Change float values to 6 digits
        latitude = (tempLat - 100*(int)(tempLat/100))/60.0;
        latitude += (int)(tempLat/100);

        double tempLon = ((atof)(lon));
        longitude = (tempLon - 100*(int)(tempLon/100))/60.0;
        longitude += (int)(tempLon/100);
        longitude = -longitude;

        return true;
    }
    else
    {
        setLED(2,0);
        latitude = 0.0;
        longitude = 0.0;
        return false;
    }
}

//Function which returns the bearing angle
double HeadingAngle(double latDest, double lonDest)
{
    if (latDest == INVALID_COORDINATES && lonDest == INVALID_COORDINATES)
        return 0;

    double deltaLon = lonDest - longitude;
    double x = cos(latDest * PI/180.0) * sin(deltaLon * PI/180.0);
    double y = cos(latitude * PI/180.0) * sin(latDest * PI/180.0)
              - sin(latitude * PI/180.0) * cos(latDest * PI/180.0) * cos(deltaLon * PI/180.0);

    double Heading = (atan2(x,y) * 180.0/PI);

    if(Heading < 0.0)
        return 360.0 + Heading;

    return Heading;
}

double calculate_target_distance(double latDest, double lonDest, double curLat, double curLong)
{
    if(latDest == INVALID_COORDINATES && lonDest == INVALID_COORDINATES)
        return 0;

    double deltaLat = latDest - curLat;
    double deltaLon = lonDest - curLong;
    double A = pow(sin((deltaLat/2.0) * (PI/180.0)), 2.0)
              + (cos(latDest * PI/180.0) * cos(curLat * PI/180.0) * pow(sin(deltaLon/2.0 * (PI/180.0)), 2.0));
    double C = 2.0 * atan2(sqrt(A), sqrt(1-A));
    return R * C * 1000; //1000 for meters
}


double getLatitude(void)
{
    return latitude;
}

double getLongitude(void)
{
    return longitude;
}

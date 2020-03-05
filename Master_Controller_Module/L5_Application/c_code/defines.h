/*
 * defines.h
 *
 *  Created on: Apr 17, 2019
 *      Author: Aakash
 */

#ifndef DEFINES_H_
#define DEFINES_H_

#ifdef __cplusplus
extern "C" {
#endif

#define BRIDGE_MID                  103
#define SENSOR_MID                  105
#define CAR_CONTROL_MID             104
#define MOTOR_MID                   106
#define BRIDGE_CHECKPOINTS_MID      107
#define GPS_LOCATION_MID            108
#define COMPASS_MID                 109
#define MASTER_HEARTBEAT_MID        110
#define SENSOR_HEARTBEAT_MID        111
#define MOTOR_HEARTBEAT_MID         112
#define GPS_HEARTBEAT_MID           113
#define BRIDGE_HEARTBEAT_MID        114

#define CAN_BUS                             can1
#define CAN_BAUD_RATE                       100
#define CAN_RX_QUEUE_SIZE                   15
#define CAN_TX_QUEUE_SIZE                   10

#define HEART_BEAT_cmd                      1

#define OBSTACLE_TRACK_FWD_THRESHOLD        5
#define CLEARANCE_TRACK_THRESHOLD           OBSTACLE_TRACK_FWD_THRESHOLD - 1
#define OBSTACLE_TRACK_REAR_THRESHOLD       OBSTACLE_TRACK_FWD_THRESHOLD - 3

#define LIDAR_OFFSET_FROM_CENTER            28

#define LIDAR_ZONE_NEAR_CM                  50
#define LIDAR_ZONE_MID_CM                   100
#define LIDAR_ZONE_FAR_CM                   150

#define LIDAR_ZONE_NEAR_THRESHOLD           3
#define LIDAR_ZONE_MID_THRESHOLD            5
#define LIDAR_ZONE_FAR_THRESHOLD            7

#define ULTRASONIC_ZONE_VERY_NEAR           37
#define ULTRASONIC_ZONE_NEAR                47
#define ULTRASONIC_ZONE_MID                 97
#define ULTRASONIC_ZONE_FAR                 147

#define VERY_NEAR                           0
#define NEAR                                1
#define MID                                 2
#define FAR                                 3
#define NO_OBSTACLE                         4

#define MOTOR_VERY_SLOW_KPH                 0.30
#define MOTOR_SLOW_KPH                      0.40
#define MOTOR_MED_KPH                       0.50
#define MOTOR_FAST_KPH                      0.60
#define MOTOR_STOP_KPH                      0.00

#define FULL_LEFT                           -90.0
#define SLIGHT_LEFT                         -30.0
#define STRAIGHT                            0.0
#define SLIGHT_RIGHT                        30.0
#define FULL_RIGHT                          90.0

#define BRIDGE_LED                          0
#define SENSOR_LED                          1
#define MOTOR_LED                           2
#define GEO_LED                             3

#ifdef __cplusplus
}
#endif

#endif /* DEFINES_H_ */

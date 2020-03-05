/*
 * rplidar_protocol.h
 *
 *  Created on: Apr 24, 2019
 *      Author: User
 */

#ifndef RPLIDAR_PROTOCOL_H_
#define RPLIDAR_PROTOCOL_H_

#define RPLIDAR_CMD_SYNC_BYTE        0xA5
#define RPLIDAR_CMDFLAG_HAS_PAYLOAD  0x80


#define RPLIDAR_ANS_SYNC_BYTE1       0xA5
#define RPLIDAR_ANS_SYNC_BYTE2       0x5A

#define RPLIDAR_ANS_PKTFLAG_LOOP     0x1

typedef struct _rplidar_cmd_packet_t {
    uint8_t syncByte; //must be RPLIDAR_CMD_SYNC_BYTE
    uint8_t cmd_flag;
    uint8_t size;
    uint8_t data[0];
} __attribute__((packed)) rplidar_cmd_packet_t;


typedef struct _rplidar_ans_header_t {
    uint8_t  syncByte1; // must be RPLIDAR_ANS_SYNC_BYTE1
    uint8_t  syncByte2; // must be RPLIDAR_ANS_SYNC_BYTE2
    uint32_t size:30;
    uint32_t subType:2;
    uint8_t  type;
} __attribute__((packed)) rplidar_ans_header_t;


#endif /* RPLIDAR_PROTOCOL_H_ */

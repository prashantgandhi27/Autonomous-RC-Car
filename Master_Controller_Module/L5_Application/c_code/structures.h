/*
 * structures.h
 *
 *  Created on: Apr 17, 2019
 *      Author: Aakash
 */

#ifndef STRUCTURES_H_
#define STRUCTURES_H_

#ifdef __cplusplus
extern "C" {
#endif

typedef struct
{
    bool front;
    bool frontleft;
    bool frontright;
    bool rear;
    bool rearright;
    bool rearleft;
    bool no_obstacle;
} obstacle_detection_t;


#ifdef __cplusplus
}
#endif

#endif /* STRUCTURES_H_ */

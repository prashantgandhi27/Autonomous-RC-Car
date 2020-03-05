/*
 * LidarTask_create.h
 *
 *  Created on: Apr 7, 2019
 *      Author: halak
 */

#ifndef LIDARTASK_CREATE_H_
#define LIDARTASK_CREATE_H_

#include "scheduler_task.hpp"

#define Period_in_Hz 50

class LidarTask : public scheduler_task
{
    public:
        LidarTask(uint8_t priority);
        bool init(void);
        bool run(void *p);
};



#endif /* LIDARTASK_CREATE_H_ */

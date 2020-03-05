/*
 * LidarTask.h
 *
 *  Created on: Apr 26, 2019
 *      Author: User
 */

#ifndef LIDARTASK_H_
#define LIDARTASK_H_

#include "scheduler_task.hpp"

class LidarTask : public scheduler_task
{
    public:
        LidarTask(uint8_t priority);
        bool init(void);
        bool run(void *p);
};

#endif /* LIDARTASK_H_ */

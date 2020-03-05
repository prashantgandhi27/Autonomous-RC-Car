/*
 * queue.c
 *
 *  Created on: Mar 3, 2019
 *      Author: halak
 */
#include "queue.h"


void queue_init(queue_S *queue)
{
    queue->Max = 100;
    queue->front = 0;
    queue->rear =  (queue->Max) - 1;
    queue->count = 0;

}


bool queue_push(queue_S *queue, uint8_t push_value)
{
    if(queue->Max == queue->count)
    {
        return false;

    }
    else{
        queue->queue_memory[queue->rear] = push_value;
        queue->rear = (queue->rear + 1)%queue->Max;
        (queue->count)++;
        return true;
    }
}

bool queue_pop(queue_S *queue,uint8_t *pop_value)
{
    if(queue->count == 0)
    {
        return false;
    }
    else{
        *pop_value = queue->queue_memory[queue->front];
        queue->front = (queue->front + 1)%queue->Max;

        (queue->count)--;
        return true;
    }
}

size_t queue_get_count(queue_S *queue)
{
    return queue->count;

}


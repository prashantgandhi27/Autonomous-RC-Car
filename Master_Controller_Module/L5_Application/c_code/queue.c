/*
 * queue.c
 *
 *  Created on: Mar 2, 2019
 *      Author: Aakash
 */

#include "queue.h"
#include <stdlib.h>
#include <string.h>
#include <stdio.h>

void queueInit(sQueue *queue)
{
    memset(&queue->queue_memory, '\0', sizeof(queue->queue_memory));
    queue->front = queue->size = 0u;
    queue->end = QUEUE_CAPACITY - 1u;
}

bool queuePush(sQueue *queue, uint8_t pushValue)
{
    if (queue->size != QUEUE_CAPACITY)
    {
        queue->end = (queue->end + 1) % QUEUE_CAPACITY;
        queue->queue_memory[queue->end] = pushValue;
        queue->size += 1;
//        printf("%c pushed to queue\n", pushValue);
        return true;
    }
    else
    {
        printf("Error: Queue is full...\n");
        return false;
    }
}

bool queuePop(sQueue *queue, uint8_t *popValue)
{
    if (popValue == NULL)
    {
        return false;
    }

    if (queue->size > 0)
    {
        *popValue = queue->queue_memory[queue->front];
        queue->front = (queue->front + 1) % QUEUE_CAPACITY;
        printf("%c popped out of queue\n", *popValue);
        queue->size -= 1;
        return true;
    }
    else
    {
        printf("Error: Queue is empty...\n");
        return false;
    }
}

size_t queueGetCount(sQueue *queue)
{
    printf("queueGetCount -> %du\n", queue->size);
    return (queue->size);
}


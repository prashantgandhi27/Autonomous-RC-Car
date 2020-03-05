/*
 * queue.h
 *
 *  Created on: Mar 2, 2019
 *      Author: Aakash
 */

#ifndef QUEUE_H_
#define QUEUE_H_

#include <stdint.h>
#include <stdbool.h>
#include <stddef.h>

#define QUEUE_CAPACITY 100

typedef struct {
    size_t front, end, size;
    uint8_t queue_memory[QUEUE_CAPACITY];
} sQueue;

void queueInit(sQueue *queue);
bool queuePush(sQueue *queue, uint8_t pushValue);
bool queuePop(sQueue *queue, uint8_t *popValue);
size_t queueGetCount(sQueue *queue);

#endif /* QUEUE_H_ */

/*
 * queue.h
 *
 *  Created on: Mar 3, 2019
 *      Author: halak
 */

#ifndef QUEUE_H_
#define QUEUE_H_

#include <stdint.h>
#include <stdbool.h>
#include <stdio.h>

typedef struct {
  uint8_t queue_memory[100];
  int front;
  int rear;
  int count;
  int Max;

  // TODO: Add more members as needed
} queue_S;

void queue_init(queue_S *queue);

bool queue_push(queue_S *queue, uint8_t push_value);

bool queue_pop(queue_S *queue,uint8_t *pop_value);

size_t queue_get_count(queue_S *queue);


#endif /* QUEUE_H_ */

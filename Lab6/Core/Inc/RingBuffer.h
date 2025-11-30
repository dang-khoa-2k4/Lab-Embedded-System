/*
 * RingBuffer.h
 *
 *  Created on: Nov 7, 2025
 *      Author: hoquo
 */

#ifndef INC_RINGBUFFER_H_
#define INC_RINGBUFFER_H_
#include "main.h"
#include <stdlib.h>

typedef struct RingBuffer {
	uint16_t buffer_size;
	uint16_t * buffer;
	int head, tail;
};

void RingBuffer_init (struct RingBuffer * buffer, uint16_t buffer_size);
int RingBuffer_append (struct RingBuffer * buffer, uint16_t value);
int RingBuffer_pop (struct RingBuffer * buffer);
int RingBuffer_remove (struct RingBuffer * buffer, int item);
int RingBuffer_hasItem(struct RingBuffer * buffer, int item);
int RingBuffer_getSize(struct RingBuffer * buffer);
#endif /* INC_RINGBUFFER_H_ */

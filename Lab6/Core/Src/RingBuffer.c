/*
 * RingBuffer.c
 *
 *  Created on: Nov 7, 2025
 *      Author: hoquo
 */
#include "RingBuffer.h"

void RingBuffer_init(struct RingBuffer * buffer, uint16_t buffer_size){
		buffer->buffer_size = buffer_size;
		buffer->buffer = malloc (sizeof(uint16_t) * buffer_size);
		buffer->head = -1;
		buffer->tail = -1;
}

int RingBuffer_append (struct RingBuffer * buffer, uint16_t value)
{
	if (buffer->buffer == NULL ||
	   ((buffer->head != -1 && buffer->tail != -1) &&
	     buffer->head == (buffer->tail + 1) % buffer->buffer_size))
	{
		return -1;
	}

	// Assign new position for tail node and head node (if needed)
	if (buffer->head == -1 && buffer->tail == -1)
		buffer->head = buffer->tail = 0;
	else
		buffer->tail = (buffer->tail + 1) % buffer->buffer_size;

	buffer->buffer[buffer->tail] = value;
	return buffer->tail;
}

int RingBuffer_pop (struct RingBuffer * buffer)
{
	uint16_t value;
	if (buffer->buffer == NULL ||
		buffer->head == -1 || buffer->tail == -1)
	{
		return -1;
	}

	value = buffer->buffer[buffer->head];
	if (buffer->head == buffer->tail)
		buffer->head = buffer->tail = -1;
	else
		buffer->head = (buffer->head + 1) % buffer->buffer_size;
	return value;
}

int RingBuffer_remove (struct RingBuffer * buffer, int item)
{
	if (buffer->buffer == NULL)	return -1;
	int idx = RingBuffer_hasItem(buffer , item);
	if (idx != -1)
	{
		while (idx != buffer->tail)
		{
			int next_idx = (idx + 1) % buffer->buffer_size;
			buffer->buffer[idx] = buffer->buffer[next_idx];
			idx = next_idx;
		}
	}
	else	return -1;

	if (buffer->head == buffer->tail)
		buffer->head = buffer->tail = -1;
	else
		buffer->tail = (buffer->tail - 1 < 0)? buffer->buffer_size - 1 : buffer->tail - 1;
	return 1;
}

int RingBuffer_hasItem(struct RingBuffer * buffer, int item)
{
	if (buffer->buffer == NULL || (buffer->head == -1 && buffer->tail == -1))	return -1;

	for (int idx = buffer->head; idx != (buffer->tail + 1) % buffer->buffer_size; idx = (idx + 1) % buffer->buffer_size)
	{
		if (buffer->buffer[idx] == item)
			return idx;
	}
	return -1;
}

int RingBuffer_getSize(struct RingBuffer * buffer)
{
	if (buffer->buffer == NULL)		return -1;
	if (buffer->head == -1 && buffer->tail == -1)	return 0;
	return (buffer->tail - buffer->head + buffer->buffer_size) % buffer->buffer_size + 1;
}

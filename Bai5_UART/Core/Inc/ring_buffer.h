#ifndef INC_RING_BUFFER_H_
#define INC_RING_BUFFER_H_

#include <stdint.h>

#define RING_BUFFER_SIZE 256

typedef struct {
    uint8_t buffer[RING_BUFFER_SIZE];
    uint16_t head;
    uint16_t tail;
    uint16_t count;
} RingBuffer;

void ringBuffer_Init(RingBuffer *rb);
uint8_t ringBuffer_Write(RingBuffer *rb, uint8_t data);
uint8_t ringBuffer_Read(RingBuffer *rb, uint8_t *data);
uint8_t ringBuffer_IsEmpty(RingBuffer *rb);
uint8_t ringBuffer_IsFull(RingBuffer *rb);
uint16_t ringBuffer_Available(RingBuffer *rb);

#endif /* INC_RING_BUFFER_H_ */
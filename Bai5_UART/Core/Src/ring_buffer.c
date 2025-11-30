#include "ring_buffer.h"

void ringBuffer_Init(RingBuffer *rb) {
    rb->head = 0;
    rb->tail = 0;
    rb->count = 0;
}

uint8_t ringBuffer_Write(RingBuffer *rb, uint8_t data) {
    if (ringBuffer_IsFull(rb)) {
        return 0; // Buffer full
    }
    
    rb->buffer[rb->head] = data;
    rb->head = (rb->head + 1) % RING_BUFFER_SIZE;
    rb->count++;
    
    return 1; // Success
}

uint8_t ringBuffer_Read(RingBuffer *rb, uint8_t *data) {
    if (ringBuffer_IsEmpty(rb)) {
        return 0; // Buffer empty
    }
    
    *data = rb->buffer[rb->tail];
    rb->tail = (rb->tail + 1) % RING_BUFFER_SIZE;
    rb->count--;
    
    return 1; // Success
}

uint8_t ringBuffer_IsEmpty(RingBuffer *rb) {
    return (rb->count == 0);
}

uint8_t ringBuffer_IsFull(RingBuffer *rb) {
    return (rb->count >= RING_BUFFER_SIZE);
}

uint16_t ringBuffer_Available(RingBuffer *rb) {
    return rb->count;
}
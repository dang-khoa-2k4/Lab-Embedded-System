#include "ring_buffer.h"

void ring_buffer_init(ring_buffer_t* rb) {
    memset(rb->buffer, 0, RING_BUFFER_SIZE);
    rb->head = 0;
    rb->tail = 0;
    rb->data_available = 0;
}

uint8_t ring_buffer_put(ring_buffer_t* rb, uint8_t data) {
    uint16_t next = (rb->head + 1) % RING_BUFFER_SIZE;
    
    if (next != rb->tail) {
        rb->buffer[rb->head] = data;
        rb->head = next;
        rb->data_available = 1;
        return 1;
    }
    return 0; // Buffer full
}

uint8_t ring_buffer_get(ring_buffer_t* rb, uint8_t* data) {
    if (rb->head == rb->tail) {
        rb->data_available = 0;
        return 0; // Buffer empty
    }
    
    *data = rb->buffer[rb->tail];
    rb->tail = (rb->tail + 1) % RING_BUFFER_SIZE;
    
    if (rb->head == rb->tail) {
        rb->data_available = 0;
    }
    
    return 1;
}

uint16_t ring_buffer_available(ring_buffer_t* rb) {
    return (rb->head - rb->tail + RING_BUFFER_SIZE) % RING_BUFFER_SIZE;
}
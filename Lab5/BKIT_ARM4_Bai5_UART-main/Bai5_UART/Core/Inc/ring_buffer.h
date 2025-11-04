#ifndef INC_RING_BUFFER_H_
#define INC_RING_BUFFER_H_

#include "main.h"
#include <string.h>

#define RING_BUFFER_SIZE 128

// Ring buffer structure
typedef struct {
    uint8_t buffer[RING_BUFFER_SIZE];
    volatile uint16_t head;
    volatile uint16_t tail;
    volatile uint8_t data_available;
} ring_buffer_t;

void ring_buffer_init(ring_buffer_t* rb);
uint8_t ring_buffer_put(ring_buffer_t* rb, uint8_t data);
uint8_t ring_buffer_get(ring_buffer_t* rb, uint8_t* data);
uint16_t ring_buffer_available(ring_buffer_t* rb);

#endif /* INC_RING_BUFFER_H_ */
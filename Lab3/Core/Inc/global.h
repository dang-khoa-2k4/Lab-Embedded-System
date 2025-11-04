#ifndef INC_GLOBAL_H_
#define INC_GLOBAL_H_

#include "main.h"
#include <stdint.h>

/* System Mode Definition */
typedef enum {
    MODE_NORMAL = 0,
    MODE_MOD_RED,
    MODE_MOD_GREEN,
    MODE_MOD_YELLOW
} SystemMode;

/* Traffic State Definition */
typedef enum {
    STATE_GREEN1_RED2 = 0,  // Road 1: Green, Road 2: Red
    STATE_YELLOW1_RED2,      // Road 1: Yellow, Road 2: Red
    STATE_RED1_GREEN2,       // Road 1: Red, Road 2: Green
    STATE_RED1_YELLOW2,      // Road 1: Red, Road 2: Yellow
    STATE_RED1_RED2          // Emergency state: All Red
} TrafficState;

/* Global System Variables */
extern SystemMode current_mode;
extern TrafficState traffic_state;

/* Cycle Times (in seconds) */
extern uint8_t red_duration;
extern uint8_t green_duration;
extern uint8_t yellow_duration;

/* Modification Mode */
extern uint8_t temp_duration;
extern uint8_t blink_state;

/* State Counter */
extern uint16_t state_counter;

#endif /* INC_GLOBAL_H_ */
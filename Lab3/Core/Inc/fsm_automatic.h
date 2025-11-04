#ifndef INC_FSM_AUTOMATIC_H_
#define INC_FSM_AUTOMATIC_H_

#include "global.h"
#include "error.h"
#include <stdint.h>

/* FSM State Validation */
int is_valid_traffic_state(TrafficState state);

/* FSM Initialization */
void fsm_automatic_init(void);

/* FSM Main Loop */
void fsm_automatic_run(void);

/* FSM State Transition */
TrafficState get_next_state(TrafficState current);

/* Duration Functions */
uint8_t get_current_state_duration(void);
void validate_durations(void);

#endif /* INC_FSM_AUTOMATIC_H_ */
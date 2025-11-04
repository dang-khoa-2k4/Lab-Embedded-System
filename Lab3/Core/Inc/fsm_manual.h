#ifndef INC_FSM_MANUAL_H_
#define INC_FSM_MANUAL_H_

#include "global.h"
#include "error.h"
#include <stdint.h>

/* FSM Manual Mode Initialization */
void fsm_manual_init(void);

/* FSM Manual Mode Main Loop */
void fsm_manual_run(void);

/* Button Handlers */
void handle_button_mode(void);
void handle_button_increase(void);
void handle_button_confirm(void);

/* Duration Validation */
int validate_duration(uint8_t duration);

#endif /* INC_FSM_MANUAL_H_ */
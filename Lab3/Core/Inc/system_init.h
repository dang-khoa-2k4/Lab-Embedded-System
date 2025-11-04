#ifndef INC_SYSTEM_INIT_H_
#define INC_SYSTEM_INIT_H_

#include "error.h"
#include <stdint.h>

/* Initialization Status */
extern uint8_t init_complete;

/* Peripheral Initialization Functions */
ErrorCode peripheral_init(void);    // GPIO, FSMC, SPI, TIM
ErrorCode timer_system_init(void);  // Initialize all software timers
ErrorCode button_system_init(void); // Initialize button module
ErrorCode display_system_init(void);// Initialize display
ErrorCode fsm_system_init(void);    // Initialize FSM

/* Main System Initialization */
ErrorCode system_init(void);

/* Initialization Check */
int is_system_initialized(void);

#endif /* INC_SYSTEM_INIT_H_ */
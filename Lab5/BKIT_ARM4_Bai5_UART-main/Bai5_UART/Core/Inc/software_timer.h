/*
 * software_timer.h
 *
 *  Created on: Sep 24, 2023
 *      Author: HaHuyen
 */

#ifndef INC_SOFTWARE_TIMER_H_
#define INC_SOFTWARE_TIMER_H_

#include "tim.h"

// Timer flags
extern uint16_t flag_timer2;  // Main processing timer (50ms)
extern uint16_t flag_timer3;  // Blink timer (500ms)
extern uint16_t flag_timer4;  // Timeout check timer (100ms)

// Timer function prototypes
void timer_init(void);
void setTimer2(uint16_t duration);  // Duration in 10ms units
void setTimer3(uint16_t duration);  // Duration in 10ms units
void setTimer4(uint16_t duration);  // Duration in 10ms units

// Timer default values
#define TIMER_TICK_MS      10
#define MAIN_TIMER_MS      50   // Timer2 period
#define BLINK_TIMER_MS     500  // Timer3 period
#define TIMEOUT_TIMER_MS   100  // Timer4 period

#endif /* INC_SOFTWARE_TIMER_H_ */

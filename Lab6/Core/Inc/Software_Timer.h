/*
 * Software_Timer.h
 *
 *  Created on: Nov 16, 2025
 *      Author: hoquo
 */

#ifndef INC_SOFTWARE_TIMER_H_
#define INC_SOFTWARE_TIMER_H_
#include "main.h"
#include "tim.h"
#include "RingBuffer.h"

#define MAX_TIMERS 		(1 << 8) - 1
#define TIMER_CYCLE		1

uint8_t timerInit(void);
uint8_t timerSet(uint8_t timer_id, uint32_t duration);
void timerReset(uint8_t timer_id);
uint8_t timerTimeout(uint8_t timer_id);
uint8_t timerIsSet(uint8_t timer_id);
void timerRun(void);
#endif /* INC_SOFTWARE_TIMER_H_ */

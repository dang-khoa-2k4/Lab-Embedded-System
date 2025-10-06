/*
 * software_timer.h
 *
 *  Created on: Sep 24, 2023
 *      Author: HaHuyen
 */

#ifndef INC_SOFTWARE_TIMER_H_
#define INC_SOFTWARE_TIMER_H_

#include "main.h"

extern uint16_t flag_timer2;
extern uint16_t scan_freq; // default 1ms

void timer_init();
void setTimer2(uint16_t duration);

void set_scan_frequency(uint16_t freq);

#endif /* INC_SOFTWARE_TIMER_H_ */

// 100 ms 

// 25 = 1 / 25
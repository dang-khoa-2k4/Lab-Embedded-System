/*
 * button.h
 *
 *  Created on: Sep 25, 2023
 *      Author: HaHuyen
 */

#ifndef INC_BUTTON_H_
#define INC_BUTTON_H_

#include "spi.h"
#include "gpio.h"
#define BUTTON_IS_PRESSED 0
#define BUTTON_IS_RELEASED 1

#define PRESS_DURATION 40       // 2000ms / 50ms = 40 lần
#define AUTO_INCREMENT_PERIOD 4  // 200ms / 50ms = 4 lần

extern uint16_t button_count[16];
extern uint16_t button_hold[16];
void button_init();
void button_Scan();

#endif /* INC_BUTTON_H_ */


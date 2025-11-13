/*
 * led7_seg.h
 *
 *  Created on: Nov 3, 2025
 *      Author: a5159165
 */

#ifndef INC_LED7_SEG_H_
#define INC_LED7_SEG_H_

#include "spi.h"

void led7_init();
void led7_Scan();
void led7_SetDigit(int num, int position, uint8_t show_dot);
void led7_SetColon(uint8_t status);

void led_On(uint8_t index);
void led_Off(uint8_t index);

#endif /* INC_LED7_SEG_H_ */

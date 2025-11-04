/*
 * software_timer.c
 *
 *  Created on: Sep 24, 2023
 *      Author: HaHuyen
 */

#include "software_timer.h"

#define TIMER_CYCLE_MS 10  // TIM2 interrupt period = 10ms

// Timer flags - Exported for other modules
uint16_t flag_timer2 = 0;  // Main processing flag (50ms)
uint16_t flag_timer3 = 0;  // Blink flag (500ms)
uint16_t flag_timer4 = 0;  // Timeout check flag (100ms)

// Timer counters
uint16_t timer2_counter = 0;
uint16_t timer3_counter = 0;
uint16_t timer4_counter = 0;

uint16_t timer2_MUL = 0;
uint16_t timer3_MUL = 0;
uint16_t timer4_MUL = 0;

void timer_init(){
	HAL_TIM_Base_Start_IT(&htim2);
}

void setTimer2(uint16_t duration) {
    timer2_counter = duration / TIMER_CYCLE_MS;  // Convert ms to ticks
    timer2_MUL = timer2_counter;  // Save for auto-reload
    flag_timer2 = 0;
}

void setTimer3(uint16_t duration) {
    timer3_counter = duration / TIMER_CYCLE_MS;
    timer3_MUL = timer3_counter;
    flag_timer3 = 0;
}

void setTimer4(uint16_t duration) {
    timer4_counter = duration / TIMER_CYCLE_MS;
    timer4_MUL = timer4_counter;
    flag_timer4 = 0;
}

void HAL_TIM_PeriodElapsedCallback(TIM_HandleTypeDef *htim){
	if(htim->Instance == TIM2){
		// Timer2 - Main processing
		if(timer2_counter > 0){
			timer2_counter--;
			if(timer2_counter == 0) {
				flag_timer2 = 1;
				timer2_counter = timer2_MUL;
			}
		}
		
		// Timer3 - Blink
		if(timer3_counter > 0){
			timer3_counter--;
			if(timer3_counter == 0) {
				flag_timer3 = 1;
				timer3_counter = timer3_MUL;
			}
		}
		
		// Timer4 - Timeout
		if(timer4_counter > 0){
			timer4_counter--;
			if(timer4_counter == 0) {
				flag_timer4 = 1;
				timer4_counter = timer4_MUL;
			}
		}
		
		led7_Scan();
	}
}


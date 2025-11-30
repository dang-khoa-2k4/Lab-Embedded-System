/*
 * software_timer.c
 *
 *  Created on: Sep 24, 2023
 *      Author: HaHuyen
 */

#include "software_timer.h"

#define TIMER_CYCLE_2 1

//software timer variable
uint16_t flag_timer2 = 0;
uint16_t timer2_counter = 0;
uint16_t timer2_MUL = 0;

uint32_t count_scan = 0;
uint16_t scan_freq = 1; // default 1ms
/**
  * @brief  Init timer interrupt
  * @param  None
  * @retval None
  */
void timer_init(){
	HAL_TIM_Base_Start_IT(&htim2);
}


/**
  * @brief  Set duration of software timer interrupt
  * @param  duration Duration of software timer interrupt
  * @retval None
  */
void setTimer2(uint16_t duration){
	timer2_MUL = duration/TIMER_CYCLE_2;
	timer2_counter = timer2_MUL;
	flag_timer2 = 0;
}

/**
  * @brief  Timer interrupt routine
  * @param  htim TIM Base handle
  * @note	This callback function is called by system
  * @retval None
  */
void HAL_TIM_PeriodElapsedCallback(TIM_HandleTypeDef *htim){
	if(htim->Instance == TIM2){
		if(timer2_counter > 0){
			timer2_counter--;
			if(timer2_counter == 0) {
				flag_timer2 = 1;
				timer2_counter = timer2_MUL;
			}
		}
		// 1ms interrupt here
		count_scan++; // every 1ms
		if (count_scan >= scan_freq)
		{
			led7_Scan();
			count_scan = 0;
		}
	}
}

void set_scan_frequency(uint16_t freq)
{
    // freq in Hz
    if(freq > 0 && freq <= 1000) {
        scan_freq = 1000 / freq / 4; // in ms
    }
}

// 1 hz = 1000ms
// 25 hz = 1/25 = 40ms
// 50 hz = 1/50 = 20ms
// 100 hz = 1/100 = 10ms

/*
 * Software_Timer.c
 *
 *  Created on: Nov 16, 2025
 *      Author: hoquo
 */
#include "Software_Timer.h"

static struct RingBuffer timer_occupy;
static uint32_t timer_count[MAX_TIMERS] = {0};
static uint8_t timer_flag[MAX_TIMERS] = {0};

uint8_t timerInit(void)
{
	RingBuffer_init(&timer_occupy, MAX_TIMERS);
	HAL_TIM_Base_Start_IT(&htim2);
	HAL_TIM_Base_Start_IT(&htim4);
}

uint8_t timerSet(uint8_t timer_id, uint32_t duration)
{
	// Check if timer is set or invalid timer id
	if (timerIsSet(timer_id))	return 0;

	timer_count[timer_id] = duration / TIMER_CYCLE;
	timer_flag[timer_id] = 0;
	RingBuffer_append(&timer_occupy, timer_id);
	return 1;
}

void timerReset(uint8_t timer_id)
{
	if (timer_id >= MAX_TIMERS)	return;

	timer_count[timer_id] = 0;
	timer_flag[timer_id] = 0;
	if (RingBuffer_hasItem(&timer_occupy, timer_id) != -1)
		RingBuffer_remove(&timer_occupy, timer_id);
}

uint8_t timerTimeout(uint8_t timer_id)
{
	if (timer_id >= MAX_TIMERS)	return 0;
	return timer_flag[timer_id];
}

uint8_t timerIsSet(uint8_t timer_id)
{
	if (timer_id >= MAX_TIMERS)	return 0;
	return RingBuffer_hasItem(&timer_occupy, timer_id) != -1;
}

void timerRun(void)
{
	int size = RingBuffer_getSize(&timer_occupy);
//	HAL_GPIO_WritePin(DEBUG_LED_GPIO_Port, DEBUG_LED_Pin, 1);
	if (size == -1 || size == 0)
		return;
	for (int i = 0; i < size; i++)
	{
		int timer_id = RingBuffer_pop(&timer_occupy);
		if (timer_count[timer_id])
			timer_flag[timer_id] = --timer_count[timer_id] <= 0;
		RingBuffer_append(&timer_occupy, timer_id);
	}
}

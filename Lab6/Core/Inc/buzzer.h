/*
 * buzzer.h
 */

#ifndef INC_BUZZER_H_
#define INC_BUZZER_H_

/* Includes */
#include "main.h"
#include "tim.h"
#include <stdint.h>

//TIM_HandleTypeDef htim13;

/* Functions */
void buzzer_init();
void buzzer_set_volume(uint8_t duty_cycle);

#endif /* INC_BUZZER_H_ */

/*
 * lab1.h
 *
 *  Created on: 2025
 *      Author: 2211635 - Nguyen Tran Dang Khoa
 *      		2210077 - Ngo Duc Anh
 */

#ifndef INC_LAB1_H_
#define INC_LAB1_H_

#include "main.h"
#include "stdint.h"
#include "software_timer.h"

// Define part 
#define LED_ON 1
#define LED_OFF 0

#define DURATION 100 //ms

// define for 7 segment 
#define NUM_0   (SEG_0_Pin | SEG_1_Pin | SEG_2_Pin | SEG_3_Pin | SEG_4_Pin | SEG_5_Pin)
#define NUM_1   (SEG_1_Pin | SEG_2_Pin)
#define NUM_2   (SEG_0_Pin | SEG_1_Pin | SEG_3_Pin | SEG_4_Pin | SEG_6_Pin)
#define NUM_3   (SEG_0_Pin | SEG_1_Pin | SEG_2_Pin | SEG_3_Pin | SEG_6_Pin)
#define NUM_4   (SEG_1_Pin | SEG_2_Pin | SEG_5_Pin | SEG_6_Pin)
#define NUM_5   (SEG_0_Pin | SEG_2_Pin | SEG_3_Pin | SEG_5_Pin | SEG_6_Pin)
#define NUM_6   (SEG_0_Pin | SEG_2_Pin | SEG_3_Pin | SEG_4_Pin | SEG_5_Pin | SEG_6_Pin)
#define NUM_7   (SEG_0_Pin | SEG_1_Pin | SEG_2_Pin)
#define NUM_8   (SEG_0_Pin | SEG_1_Pin | SEG_2_Pin | SEG_3_Pin | SEG_4_Pin | SEG_5_Pin | SEG_6_Pin)
#define NUM_9   (SEG_0_Pin | SEG_1_Pin | SEG_2_Pin | SEG_3_Pin | SEG_5_Pin | SEG_6_Pin)
#define ALL_SEG (SEG_0_Pin | SEG_1_Pin | SEG_2_Pin | SEG_3_Pin | SEG_4_Pin | SEG_5_Pin | SEG_6_Pin)
// End define part

// Enum part
typedef enum 
{
    RED_LIGHT = 0,
    YELLOW_LIGHT = 1,
    GREEN_LIGHT = 2 
} TrafficLight;
// End enum part

void system_init() {
	  HAL_GPIO_WritePin(OUTPUT_Y0_GPIO_Port, OUTPUT_Y0_Pin, 0);
	  HAL_GPIO_WritePin(OUTPUT_Y1_GPIO_Port, OUTPUT_Y1_Pin, 0);
	  HAL_GPIO_WritePin(DEBUG_LED_GPIO_Port, DEBUG_LED_Pin, 0);
	  timer_init();
	  led7_init();
    // duration 100ms
	  setTimer2(DURATION);
}

// Function prototype part
void ex_1_chapter_1_handle();
void ex_2_chapter_1_handle();
void ex_3_chapter_1_handle();
// void display7Seg(uint8_t num);
void ex_1_chapter_2_handle();
void ex_2_chapter_2_handle();
void ex_3_chapter_2_handle();
void ex_4_chapter_2_handle();
void ex_5_chapter_2_handle();
// void clearAllClock();
// void ex_7_handle();
// void setNumberOnClock(int num);
// void ex_8_handle();
// void clearNumberOnClock(int num);
// void ex_9_handle();
// void ex_10_handle();
// End function prototype part

#endif /* INC_LAB1_H_ */
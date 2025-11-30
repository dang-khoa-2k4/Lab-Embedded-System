/*
 * lab4.h
 *
 *  Created on: Nov 17, 2025
 *      Author: hoquo
 */

#ifndef INC_LAB6_H_
#define INC_LAB6_H_
#include "main.h"

#include "gpio.h"
#include "i2c.h"
#include "spi.h"
#include "tim.h"
#include "usart.h"
#include "uart.h"

#include "ds3231.h"
#include "sensor.h"
#include "button.h"
#include "buzzer.h"
#include "utils.h"
#include "led_7seg.h"
#include "fsmc.h"
#include "lcd.h"

#include "Software_Timer.h"
#include "Scheduler.h"

typedef struct {
	uint32_t sec;
	uint32_t min;
	uint32_t hour;
	uint32_t day;
	uint32_t month;
	uint32_t yea;
	uint32_t day_of_week;
} Clock;

typedef struct {
	Clock		system_clock;

	float 		power;
	uint8_t 	light_emission;
	float		temp;
	uint16_t 	humid;
} Monitor;

void systemInit(void);
void systemRun(void);

// Sensor Functions
void updateSensorValue(void);
void alertBuzzer(void);

// RTC Functions
void setTime(void);
void updateTime(void);
void drawGraph(void);

// LCD Functions
void displayLCD(void);

// Button Functions

#endif /* INC_LAB6_H_ */

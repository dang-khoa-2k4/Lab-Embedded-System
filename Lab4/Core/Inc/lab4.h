/*
 * lab4.h
 *
 *  Created on: Nov 3, 2025
 *      Author: Khoa
 */

#ifndef INC_LAB4_H_
#define INC_LAB4_H_
#include "main.h"

typedef enum {
    YEAR = 0,
    MONTH,
    DATE, 
    DAY,
    HOUR,
    MIN,
    SEC
} TimeUnit;

uint16_t alarm_time[7] = {0, 0, 0, 0, 0, 0, 0};
uint16_t time_setting[7] = {25, 11, 3, 2, 14, 30, 0};

typedef enum {
    INIT,
    CLOCK,
    SET_TIME,
    ALARM
} State;

extern State currentState;

void displayTime();
void timeSetting();
void alarmSetting();

void fsm_init();
void fsm_clock();

void updateTime(){
	ds3231_Write(ADDRESS_YEAR, time_setting[YEAR]);
	ds3231_Write(ADDRESS_MONTH, time_setting[MONTH]);
	ds3231_Write(ADDRESS_DATE, time_setting[DATE]);
	ds3231_Write(ADDRESS_DAY, time_setting[DAY]);
	ds3231_Write(ADDRESS_HOUR, time_setting[HOUR]);
	ds3231_Write(ADDRESS_MIN, time_setting[MIN]);
	ds3231_Write(ADDRESS_SEC, time_setting[SEC]);
}

uint8_t isButtonUp()
{
    if (button_count[3] == 1)
        return 1;
    else
        return 0;
}

uint8_t isButtonE()
{
    if (button_count[11] == 1)
        return 1;
    else
        return 0;
}

uint8_t isButtonB()
{
    if (button_count[14] == 1)
        return 1;
    else
        return 0;
}

uint8_t isButtonDown()
{
    if (button_count[7] == 1)
        return 1;
    else
        return 0;
}
void displayTime();

#endif /* INC_LAB4_H_ */

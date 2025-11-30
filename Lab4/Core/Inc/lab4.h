/*
 * lab4.h
 *
 *  Created on: Nov 3, 2025
 *      Author: Khoa
 */

#ifndef INC_LAB4_H_
#define INC_LAB4_H_
#include "main.h"

const uint16_t coord_x_time[7] = {150, 110, 70, 20, 70, 110, 150};
const uint16_t coord_y_time[7] = {130, 130, 130, 130, 100, 100, 100};

uint16_t alarm_time[3] = {0, 0, 0};
uint16_t time_setting[7] = {25, 11, 3, 2, 14, 30, 0};

typedef enum {
    YEAR = 0,
    MONTH,
    DATE, 
    DAY,
    HOUR,
    MIN,
    SEC
} TimeUnit;

typedef enum {
    INIT,
    CLOCK,
    SET_TIME,
    ALARM
} State;

extern State currentState;

void displayTime();
void displayTimeWithBlink(TimeUnit unit, uint16_t time_value);
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
    if (button_count[3] > 0)
        return 1;
    else
        return 0;
}
uint8_t isButtonE()
{
    if (button_count[12] == 1)
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

#endif /* INC_LAB4_H_ */

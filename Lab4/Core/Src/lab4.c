/*
 * lab4.c
 *
 *  Created on: Nov 3, 2025
 *      Author: Khoa
 */

#include "lab4.h"

State currentState;

char modeString[30];

static void getModeString()
{
    // clear previous string
    memset(modeString, 0, sizeof(modeString));
    // set new string based on current state
    if (currentState == CLOCK)
        sprintf(modeString, "Current mode: CLOCK");
    else if (currentState == SET_TIME)
        sprintf(modeString, "Current mode: SET TIME");
    else if (currentState == ALARM)
        sprintf(modeString, "Current mode: ALARM");
}

void setupLCD()
{
    lcd_Clear(BLACK);
    lcd_Fill(0, 0, 240, 20, BLUE);
	lcd_StrCenter(0, 2, "CLOCK", RED, BLUE, 16, 1);
	lcd_ShowStr(20, 30, modeString, WHITE, RED, 24, 0);
}

void displayTime()
{
	lcd_ShowIntNum(70, 100, ds3231_hours, 2, GREEN, BLACK, 24);
	lcd_ShowIntNum(110, 100, ds3231_min, 2, GREEN, BLACK, 24);
	lcd_ShowIntNum(150, 100, ds3231_sec, 2, GREEN, BLACK, 24);
	lcd_ShowIntNum(20, 130, ds3231_day, 2, YELLOW, BLACK, 24);
	lcd_ShowIntNum(70, 130, ds3231_date, 2, YELLOW, BLACK, 24);
	lcd_ShowIntNum(110, 130, ds3231_month, 2, YELLOW, BLACK, 24);
	lcd_ShowIntNum(150, 130, ds3231_year, 2, YELLOW, BLACK, 24);
}

void timeSetting()
{
    uint8_t cur_setting = YEAR;
    uint8_t max_values[] = {99, 12, 31, 7, 23, 59, 59};

    if (isButtonUp())
    {
        time_setting[cur_setting] = 
            (time_setting[cur_setting] + 1) % (max_values[cur_setting] + 1);
    }

    if (isButtonE())
    {
        cur_setting = (cur_setting + 1) % 7;
    }
}

void alarmSetting()
{
    uint8_t cur_setting = HOUR;
    uint8_t max_values[] = {23, 59, 59};

    if (isButtonUp())
    {
        alarm_time[cur_setting] = (alarm_time[cur_setting] + 1) % (max_values[cur_setting] + 1);
    }
}

void fsm_init()
{
    currentState = CLOCK;
    getModeString();
    setupLCD();
}
void fsm_clock()
{
    switch (currentState)
    {
        case INIT:
            fsm_init();
            if (1) 
            {
                currentState = CLOCK;
                getModeString();
                setupLCD();
            }
            break;
        case CLOCK:
            displayTime(); 
            if (isButtonB())
            {
                currentState = SET_TIME;
                getModeString();
                setupLCD();
            }
            // check alarm
            if (ds3231_hours == alarm_time[HOUR] &&
                ds3231_min == alarm_time[MIN] &&
                ds3231_sec == alarm_time[SEC])
            {
                // Alarm triggered
                lcd_Fill(0, 160, 240, 200, RED);
                lcd_StrCenter(0, 170, "ALARM!!!", WHITE, RED, 24, 1);
            }
            break;
        case SET_TIME:
            timeSetting();
            if (isButtonB())
            {
                currentState = ALARM;
                getModeString();
                setupLCD();
            }
            break;
        case ALARM:
            alarmSetting();
            if (isButtonB())
            {
                currentState = CLOCK;
                getModeString();
                setupLCD();
            }
            break;
        default:
            break;
    }
}

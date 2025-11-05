/*
 * lab4.c
 *
 *  Created on: Nov 3, 2025
 *      Author: Khoa
 */

#include "lab4.h"

State currentState;

static uint8_t cur_setting;
char modeString[30];
uint8_t firstEntry = 1;
static uint32_t counter_toggle = 0;

uint16_t cur_time[7] = {0, 0, 0, 0, 0, 0, 0}; 
// Blink the selected time unit at 2Hz on LCD
static void blinkTimeUnit2Hz(TimeUnit unit, uint16_t time_value)
{
    counter_toggle = (counter_toggle + 1) % 10; // Toggle every 500ms
    static uint8_t cur_state = 0;
    if (counter_toggle == 0)
    {
        if (cur_state == 0)
            lcd_ShowIntNum(coord_x_time[unit], coord_y_time[unit], 
                       0, 2, BLACK, BLACK, 24);
        else
            lcd_ShowIntNum(coord_x_time[unit], coord_y_time[unit], 
                       time_value, 2, RED, BLACK, 24);
        cur_state = ~cur_state;
    }
}

static void getCurrentTime()
{
    cur_time[YEAR] = ds3231_year;
    cur_time[MONTH] = ds3231_month;
    cur_time[DATE] = ds3231_date;
    cur_time[DAY] = ds3231_day;
    cur_time[HOUR] = ds3231_hours;
    cur_time[MIN] = ds3231_min;
    cur_time[SEC] = ds3231_sec;
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

void displayTimeWithBlink(TimeUnit unit, uint16_t time_value)
{
    TimeUnit index = YEAR;
    for (index = YEAR; index <= SEC; index++)
    {
        if (index == unit)
            blinkTimeUnit2Hz(unit, time_value);
        else
            lcd_ShowIntNum(coord_x_time[index], coord_y_time[index], cur_time[index], 2, 
                (index > DAY) ? GREEN : YELLOW, BLACK, 24);
    }
}

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
	lcd_StrCenter(20, 30, modeString, WHITE, RED, 16, 0);
}

// Helper function to get max days in a month
static uint8_t getMaxDaysInMonth(uint8_t month, uint8_t year)
{
    uint8_t days_in_month[] = {31, 28, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31};
    
    if (month < 1 || month > 12)
        return 31;
    
    // Check for leap year (February)
    if (month == 2)
    {
        // Leap year: divisible by 4, except for years divisible by 100 unless also divisible by 400
        if ((year % 4 == 0 && year % 100 != 0) || (year % 400 == 0))
            return 29;
    }
    
    return days_in_month[month - 1];
}

void timeSetting()
{   
    
    uint8_t max_values[] = {99, 12, 31, 7, 23, 59, 59}; // year, month, date, day, hour, min, sec
    static uint16_t time_setting_temp[7];

    if (firstEntry)
    {
        memcpy(time_setting_temp, time_setting, sizeof(time_setting));
        firstEntry = 0;
    }

    displayTimeWithBlink((TimeUnit)cur_setting, time_setting_temp[cur_setting]);

    uint8_t max_date = getMaxDaysInMonth(time_setting_temp[MONTH], time_setting_temp[YEAR]);
    if (isButtonUp())
    {
        time_setting_temp[cur_setting] = 
            (time_setting_temp[cur_setting] + 1) % (max_values[cur_setting] + 1);

        // Handle special cases for date range based on month
        if (cur_setting == DATE)
        {
            if (time_setting_temp[DATE] == 0)
                time_setting_temp[DATE] = 1; // date starts from 1
            else if (time_setting_temp[DATE] > max_date)
                time_setting_temp[DATE] = 1; // wrap around to 1
        }
        
        // Handle day (1-7)
        if (cur_setting == DAY)
        {
            if (time_setting_temp[DAY] == 0)
                time_setting_temp[DAY] = 1;
        }
        
        // Handle month (1-12)
        if (cur_setting == MONTH)
        {
            if (time_setting_temp[MONTH] == 0)
                time_setting_temp[MONTH] = 1;
            else if (time_setting_temp[MONTH] > 12)
                time_setting_temp[MONTH] = 1;
            // Update with new month
            max_date = getMaxDaysInMonth(time_setting_temp[MONTH], time_setting_temp[YEAR]);
            // Adjust date if current date exceeds max days in new month
            if (time_setting_temp[DATE] > max_date)
                time_setting_temp[DATE] = max_date;
        }
        
        // When year changes, check if Feb 29 is still valid
        if (cur_setting == YEAR)
        {
            if (time_setting[MONTH] == 2 && time_setting[DATE] == 29)
            {
                uint8_t max_date = getMaxDaysInMonth(2, time_setting_temp[YEAR]);
                if (max_date < 29)
                    time_setting_temp[DATE] = 28;
            }
        }
    }
    else if (isButtonDown())
    {
        if (time_setting_temp[cur_setting] == 0)
            time_setting_temp[cur_setting] = max_values[cur_setting];
        else
            time_setting_temp[cur_setting]--;

        // Handle special cases for date range based on month
        if (cur_setting == DATE)
        {
            if (time_setting_temp[DATE] == 0)
                time_setting_temp[DATE] = max_date; // wrap around to max date
        }
        
        // Handle day (1-7)
        if (cur_setting == DAY)
        {
            if (time_setting_temp[DAY] == 0)
                time_setting_temp[DAY] = 7;
        }
        
        // Handle month (1-12)
        if (cur_setting == MONTH)
        {
            if (time_setting_temp[MONTH] == 0)
                time_setting_temp[MONTH] = 12;
            // Update with new month
            max_date = getMaxDaysInMonth(time_setting_temp[MONTH], time_setting_temp[YEAR]);
            // Adjust date if current date exceeds max days in new month
            if (time_setting_temp[DATE] > max_date)
                time_setting_temp[DATE] = max_date;
        }

        // When year changes, check if Feb 29 is still valid
        if (cur_setting == YEAR)
        {
            if (time_setting[MONTH] == 2 && time_setting[DATE] == 29)
            {
                uint8_t max_date = getMaxDaysInMonth(2, time_setting_temp[YEAR]);
                if (max_date < 29)
                    time_setting_temp[DATE] = 28;
            }
        }
    }


    if (isButtonE())
    {
        cur_time[cur_setting] = time_setting_temp[cur_setting];
        time_setting[cur_setting] = time_setting_temp[cur_setting];
        cur_setting = (cur_setting + 1) % 7;
    }
}

void alarmSetting()
{
    static uint16_t alarm_time_temp[3];
    
    // Initialize with current time on first entry to ALARM state
    if (firstEntry)
    {
        alarm_time_temp[HOUR - 4] = ds3231_hours;
        alarm_time_temp[MIN - 4] = ds3231_min;
        alarm_time_temp[SEC - 4] = ds3231_sec;
        firstEntry = 0;
    }
    // set alarm time is current time
    uint8_t max_values[] = {23, 59, 59};

    displayTimeWithBlink((TimeUnit)(cur_setting + 4), alarm_time_temp[cur_setting]);

    if (isButtonUp())
    {
        alarm_time_temp[cur_setting] = 
            (alarm_time_temp[cur_setting] + 1) % (max_values[cur_setting] + 1);
    }
    else if (isButtonDown())
    {
        if (alarm_time_temp[cur_setting] == 0)
            alarm_time_temp[cur_setting] = max_values[cur_setting];
        else
            alarm_time_temp[cur_setting]--;
    }

    if (isButtonE())
    {
        cur_time[cur_setting + 4] = alarm_time_temp[cur_setting];
        cur_setting = (cur_setting + 1) % 3;
        alarm_time[HOUR] = alarm_time_temp[HOUR];
        alarm_time[MIN] = alarm_time_temp[MIN];
        alarm_time[SEC] = alarm_time_temp[SEC];
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
                firstEntry = 1;
                currentState = SET_TIME;
                cur_setting = YEAR;
                getModeString();
                setupLCD();
                // store current time to cur_time for later use
                getCurrentTime();
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
                updateTime();
                firstEntry = 1; // reset for alarm setting
                currentState = ALARM;
                cur_setting = HOUR - 4; // for right indexing
                getModeString();
                setupLCD();
                getCurrentTime(); // update current time
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

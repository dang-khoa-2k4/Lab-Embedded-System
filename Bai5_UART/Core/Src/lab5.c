/*
 * lab5.c
 *
 *  Created on: Nov 3, 2025
 *      Author: Khoa
 */

#include "lab5.h"

State currentState;

static TimeUnit cur_setting;
static char modeString[30];
uint8_t firstEntry = 1;
static uint32_t counter_toggle = 0;
static uint32_t counter_timeout = 0;


// just for display time with blink
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

static void getStringCmd(TimeUnit cmd, char* str)
{
    switch (cmd)
    {
        case YEAR:
            sprintf(str, "YEAR");
            break;
        case MONTH:
            sprintf(str, "MONTH");
            break;
        case DATE:
            sprintf(str, "DATE");
            break;
        case DAY:
            sprintf(str, "DAY");
            break;
        case HOUR:
            sprintf(str, "HOUR");
            break;
        case MIN:
            sprintf(str, "MINUTE");
            break;
        case SEC:
            sprintf(str, "SECOND");
            break;
        default:
            sprintf(str, "UNKNOWN");
            break;
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

void displayUARTLog(TimeUnit unit)
{   
    char str[20];
    getStringCmd(unit, str);
    sprintf(str, "Updating %s...:\n", str);
    // check again in real hardware
    lcd_StrCenter(120, 30, str, WHITE, RED, 16, 0);
}

static void getModeString()
{
    // clear previous string
    memset(modeString, 0, sizeof(modeString));
    // set new string based on current state
    if (currentState == CLOCK)
        sprintf(modeString, "Mode: CLOCK");
    else if (currentState == SET_TIME)
        sprintf(modeString, "Mode: SET TIME");
    else if (currentState == ALARM)
        sprintf(modeString, "Mode: ALARM");
    else if (currentState == UPDATE_TIME_THROUGH_UART)
        sprintf(modeString, "Mode: UPDATE UART");
}

// Validate time value based on TimeUnit
static uint8_t validateTimeValue(TimeUnit unit, uint16_t value, uint16_t current_time[])
{
    switch (unit)
    {
        case YEAR:
            return (value >= 0 && value <= 99); // 2000-2099
            
        case MONTH:
            return (value >= 1 && value <= 12);
            
        case DATE:
        {
            uint8_t max_days = getMaxDaysInMonth(current_time[MONTH], current_time[YEAR]);
            return (value >= 1 && value <= max_days);
        }
        
        case DAY:
            return (value >= 1 && value <= 7); // Monday(1) to Sunday(7)
            
        case HOUR:
            return (value >= 0 && value <= 23);
            
        case MIN:
        case SEC:
            return (value >= 0 && value <= 59);
            
        default:
            return 0;
    }
}

// check reponse template (if 0 <= hour <= 12,...) from uart and process data
// Handle UART response and update time_setting
static uint8_t handleUARTResponse(TimeUnit cur_setting, uint16_t* time_setting_temp)
{
    static char uart_buffer[MAX_RESPONSE_SIZE];
    static uint8_t buffer_index = 0;
    uint8_t received_data;
    uint8_t data_received = 0;
    
    // Read all available data from ring buffer
    while (ringBuffer_Available(&rs232_rx_buffer) > 0)
    {
        if (ringBuffer_Read(&rs232_rx_buffer, &received_data))
        {
            // Echo back for confirmation
            // uart_Rs232SendBytes(&received_data, 1);
            uart_buffer[buffer_index++] = received_data;
            // Check for end of input (Enter key: '\r' or '\n')
            if (received_data == '\r' || received_data == '\n')
            {
                if (buffer_index > 0)
                {
                    uart_buffer[buffer_index] = '\0'; // Null terminate string
                    
                    // Convert string to integer
                    int value = atoi(uart_buffer);
                    
                    // Validate the value based on current setting
                    if (validateTimeValue(cur_setting, value, time_setting_temp))
                    {
                        // Update time_setting_temp with valid value
                        time_setting_temp[cur_setting] = (uint16_t)value;
                        
                        // Send success message
                        char success_msg[50];
                        char unit_str[20];
                        getStringCmd(cur_setting, unit_str);
                        sprintf(success_msg, "\r\n%s updated to %d\r\n", unit_str, value);
                        uart_Rs232SendString(success_msg);
                        
                        data_received = 1;
                    }
                    else
                    {
                        // Send error message with valid range
                        char error_msg[100];
                        char unit_str[20];
                        getStringCmd(cur_setting, unit_str);
                        
                        switch (cur_setting)
                        {
                            case YEAR:
                                sprintf(error_msg, "\r\nERROR: %s must be 0-99\r\n", unit_str);
                                break;
                            case MONTH:
                                sprintf(error_msg, "\r\nERROR: %s must be 1-12\r\n", unit_str);
                                break;
                            case DATE:
                            {
                                uint8_t max_days = getMaxDaysInMonth(time_setting_temp[MONTH], 
                                                                     time_setting_temp[YEAR]);
                                sprintf(error_msg, "\r\nERROR: %s must be 1-%d\r\n", 
                                       unit_str, max_days);
                                break;
                            }
                            case DAY:
                                sprintf(error_msg, "\r\nERROR: %s must be 1-7\r\n", unit_str);
                                break;
                            case HOUR:
                                sprintf(error_msg, "\r\nERROR: %s must be 0-23\r\n", unit_str);
                                break;
                            case MIN:
                            case SEC:
                                sprintf(error_msg, "\r\nERROR: %s must be 0-59\r\n", unit_str);
                                break;
                            default:
                                sprintf(error_msg, "\r\nERROR: Invalid input\r\n");
                                break;
                        }
                        uart_Rs232SendString(error_msg);
                        
                        // Request input again
                        char unit_str_retry[20];
                        getStringCmd(cur_setting, unit_str_retry);
                        uart_Rs232SendString(unit_str_retry);
                        uart_Rs232SendString(": ");
                    }
                    
                    // Reset buffer
                    buffer_index = 0;
                    memset(uart_buffer, 0, sizeof(uart_buffer));
                }
            }
            // Handle backspace
            else if (received_data == 0x08 || received_data == 0x7F)
            {
                if (buffer_index > 0)
                {
                    buffer_index--;
                    uart_Rs232SendString("\b \b"); // Erase character on terminal
                }
            }
            // Store valid digit characters
            else if (received_data >= '0' && received_data <= '9')
            {
                if (buffer_index < MAX_RESPONSE_SIZE - 1)
                {
                    uart_buffer[buffer_index++] = received_data;
                }
            }
        }
    }
    
    return data_received;
}

void setupLCD()
{
    lcd_Clear(BLACK);
    lcd_Fill(0, 0, 240, 20, BLUE);
	lcd_StrCenter(0, 2, "CLOCK", RED, BLUE, 16, 1);
	lcd_StrCenter(20, 30, modeString, WHITE, RED, 16, 0);
}

void timeSetting()
{   
    
    uint8_t max_values[] = {99, 12, 31, 7, 23, 59, 59}; // year, month, date, day, hour, min, sec
    static uint16_t time_setting_temp[7];

    if (firstEntry)
    {
        getCurrentTime();
        memcpy(time_setting_temp, time_setting, sizeof(time_setting));
        firstEntry = 0;
        counter_toggle = 0;
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
        getCurrentTime(); // update current time
        alarm_time_temp[HOUR - 4] = ds3231_hours;
        alarm_time_temp[MIN - 4] = ds3231_min;
        alarm_time_temp[SEC - 4] = ds3231_sec;
        firstEntry = 0;
        counter_toggle = 0;
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

void uartTimeSetting()
{
    static uint8_t time_out = 0;
    static uint16_t time_setting_temp[7];
    static uint8_t prompt_sent = 0;
    
    if (firstEntry)
    {
        getCurrentTime();
        memcpy(time_setting_temp, cur_time, sizeof(cur_time));
        firstEntry = 0;
        counter_timeout = 0;
        prompt_sent = 0;
    }
    displayTimeWithBlink((TimeUnit)cur_setting, time_setting_temp[cur_setting]);
    displayUARTLog((TimeUnit)cur_setting);
    // Timeout handling
    if (!prompt_sent)
    {
        char cmd[10];
        getStringCmd(cur_setting, cmd);
        uart_Rs232SendString("\r\n");
        uart_Rs232SendString(cmd);
        uart_Rs232SendString(": ");
        prompt_sent = 1;
        counter_timeout = 0;
    }
    //wait reponse from uart
    if (handleUARTResponse(cur_setting, time_setting_temp))
    {
        // Processed valid data
        counter_timeout = 0;
        prompt_sent = 0; // reset for next prompt
        // Move to next setting
        cur_time[cur_setting] = time_setting_temp[cur_setting];
        cur_setting = (cur_setting + 1) % 7;
        if (cur_setting == SEC) // last setting
        {
            // update all time settings
            memcpy(time_setting, time_setting_temp, sizeof(time_setting));
        }
    }
    else
    {
        counter_timeout = (counter_timeout + 1) % TIMEOUT_TICK;
        if (counter_timeout == 0)
        {
            if (++time_out > MAX_RESENT_TIMEOUT)
            {
                uart_Rs232SendString("\r\n[ERROR] Max timeout reached");
                lcd_StrCenter(120, 50, "UART TIMEOUT! Returning to CLOCK mode...", WHITE, RED, 16, 0);
                // Return to CLOCK mode
                currentState = CLOCK;
                getModeString();
                setupLCD();
            }
            // Timeout occurred, resend prompt
            uart_Rs232SendString("\r\n[TIMEOUT] ");
            lcd_StrCenter(120, 50, "UART TIMEOUT! Resending prompt...", WHITE, RED, 16, 0);
            prompt_sent = 0; // Resend prompt
        }
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
            // check alarm
            if (ds3231_hours == alarm_time[HOUR] &&
                ds3231_min == alarm_time[MIN] &&
                ds3231_sec == alarm_time[SEC])
            {
                // Alarm triggered
                lcd_Fill(0, 160, 240, 200, RED);
                lcd_StrCenter(0, 170, "ALARM!!!", WHITE, RED, 24, 1);
            }

            if (isButtonB())
            {
                firstEntry = 1;
                currentState = SET_TIME;
                cur_setting = YEAR;
                getModeString();
                setupLCD();
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
                
            }
            break;
        case ALARM:
            alarmSetting();
            if (isButtonB())
            {
                currentState = UPDATE_TIME_THROUGH_UART;
                firstEntry = 1;
                cur_setting = YEAR;
                getModeString();
                setupLCD();
                
            }
            break;
        case UPDATE_TIME_THROUGH_UART:
            uartTimeSetting();
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

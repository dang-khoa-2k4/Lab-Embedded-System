/*
 * lab1.c
 *
 *  Created on: Sep 8, 2024
 *      Author: 2211635 - Nguyen Tran Dang Khoa
 */
#include "lab1.h"

static uint8_t TRAFFIC_LIGHT_TIME[] = {5, 1, 3};
// static uint16_t TIME_CLOCK_LED[12] = {   LED_12_Pin, LED_1_Pin, LED_2_Pin , LED_3_Pin, 
//                                         LED_4_Pin , LED_5_Pin, LED_6_Pin , LED_7_Pin, 
//                                         LED_8_Pin , LED_9_Pin, LED_10_Pin, LED_11_Pin   };

void ex_1_chapter_1_handle()
{
    HAL_GPIO_WritePin(DEBUG_LED_GPIO_Port, DEBUG_LED_Pin, LED_ON);
    HAL_Delay(2000);
    HAL_GPIO_WritePin(DEBUG_LED_GPIO_Port, DEBUG_LED_Pin, LED_OFF);
    HAL_Delay(4000);
}

void ex_2_chapter_1_handle()
{
    static uint8_t state = LED_ON;
    static uint8_t count_seconds = 0;
    
    // Khởi tạo LED lần đầu tiên
    static uint8_t first_run = 1;
    if (first_run)
    {
        HAL_GPIO_WritePin(DEBUG_LED_GPIO_Port, DEBUG_LED_Pin, LED_ON);
        first_run = 0;
    }
    
    switch (state)
    {
    case LED_ON:
        if (count_seconds >= 2)  // Sáng trong 2 giây
        {
            HAL_GPIO_WritePin(DEBUG_LED_GPIO_Port, DEBUG_LED_Pin, LED_OFF);
            state = LED_OFF;
            count_seconds = 0;
        }
        break;
    case LED_OFF:
        if (count_seconds >= 4)  // Tắt trong 4 giây
        {
            HAL_GPIO_WritePin(DEBUG_LED_GPIO_Port, DEBUG_LED_Pin, LED_ON);
            state = LED_ON;
            count_seconds = 0;
        }
        break;
    }
    
    HAL_Delay(1000);
    count_seconds++;
}

static void traffic_light_handle(TrafficLight * light_state, uint16_t *leds, uint32_t * count_seconds)
{   
    uint8_t count = 0;
    HAL_GPIO_WritePin(DEBUG_LED_GPIO_Port, leds[0] | leds[1] | leds[2], LED_OFF);
    // handle for timer
    count = *count_seconds > 10 ? *count_seconds / 10 : *count_seconds;

    switch (*light_state) 
    {
    case RED_LIGHT:
        HAL_GPIO_WritePin(DEBUG_LED_GPIO_Port, leds[*light_state], LED_ON);
        if (count >= TRAFFIC_LIGHT_TIME[*light_state])
        {
            *light_state = GREEN_LIGHT;
            *count_seconds = 0;
        }
        break;
    case YELLOW_LIGHT:
        HAL_GPIO_WritePin(DEBUG_LED_GPIO_Port, leds[*light_state], LED_ON);
        if (count >= TRAFFIC_LIGHT_TIME[*light_state])
        {
            *light_state = RED_LIGHT;
            *count_seconds = 0;
        }
        break;
    case GREEN_LIGHT:
        HAL_GPIO_WritePin(DEBUG_LED_GPIO_Port, leds[*light_state], LED_ON);
        if (count >= TRAFFIC_LIGHT_TIME[*light_state])
        {
            *light_state = YELLOW_LIGHT;
            *count_seconds = 0;
        }
        break;
    default:
        break;
    }
}


void ex_3_chapter_1_handle()
{
    static TrafficLight light_state = RED_LIGHT;
    static uint32_t count_seconds = 0;
    static uint16_t leds[3] = {DEBUG_LED_Pin, OUTPUT_Y0_Pin, OUTPUT_Y1_Pin};
    traffic_light_handle(&light_state, leds, &count_seconds);
    HAL_Delay(1000);
    count_seconds++;
}

// Debug led toggle every 2 seconds
// led_y0 on 2s, off 4s 
// led_y1 on 5s, off 1s
// every 100 ms timer will be set
void ex_1_chapter_2_handle()
{
    static uint8_t count_LED_debug = 0;
    count_LED_debug = (count_LED_debug + 1) % 10;
    if (count_LED_debug == 0) {
        HAL_GPIO_TogglePin(DEBUG_LED_GPIO_Port, DEBUG_LED_Pin);
    }

    static uint8_t state_Y0 = LED_ON;
    static uint16_t count_LED_Y0 = 0;
    
    switch (state_Y0)
    {
    case LED_ON:
        if (count_LED_Y0 >= 20)  // Sáng trong 2 giây
        {
            HAL_GPIO_WritePin(OUTPUT_Y0_GPIO_Port, OUTPUT_Y0_Pin, LED_OFF);
            state_Y0 = LED_OFF;
            count_LED_Y0 = 0;
        }
        break;
    case LED_OFF:
        if (count_LED_Y0 >= 40)  // Tắt trong 4 giây
        {
            HAL_GPIO_WritePin(OUTPUT_Y0_GPIO_Port, OUTPUT_Y0_Pin, LED_ON);
            state_Y0 = LED_ON;
            count_LED_Y0 = 0;
        }
        break;
    }
    
    count_LED_Y0++;

    static uint8_t state_Y1 = LED_ON;
    static uint16_t count_LED_Y1 = 0;
    
    switch (state_Y1)
    {
    case LED_ON:
        if (count_LED_Y1 >= 50)  // Sáng trong 5 giây
        {
            HAL_GPIO_WritePin(OUTPUT_Y1_GPIO_Port, OUTPUT_Y1_Pin, LED_OFF);
            state_Y1 = LED_OFF;
            count_LED_Y1 = 0;
        }
        break;
    case LED_OFF:
        if (count_LED_Y1 >= 10)  // Tắt trong 1 giây
        {
            HAL_GPIO_WritePin(OUTPUT_Y1_GPIO_Port, OUTPUT_Y1_Pin, LED_ON);
            state_Y1 = LED_ON;
            count_LED_Y1 = 0;
        }
        break;
    }

    count_LED_Y1++;
}

void ex_2_chapter_2_handle()
{
    static TrafficLight light_state = RED_LIGHT;
    static uint32_t count_seconds = 0;
    static uint16_t leds[3] = {DEBUG_LED_Pin, OUTPUT_Y0_Pin, OUTPUT_Y1_Pin};
    count_seconds++; // 100 ms => + 1 
    if (count_seconds % 10 == 0) // 1 second
        traffic_light_handle(&light_state, leds, &count_seconds);
}

void ex_3_chapter_2_handle()
{
    set_scan_frequency(1);
    HAL_Delay(3000);
    set_scan_frequency(25);
    HAL_Delay(3000);
    set_scan_frequency(100);
    HAL_Delay(3000);
}

static void clearAllClock()
{
    for (int i = 0; i < 4; i++)
    {
        led7_SetDigit(0, i, 0);
    }
}

void ex_4_chapter_2_handle()
{
    static uint8_t hour = 10;
    static uint8_t minute = 0;
    static uint8_t second = 0;
    if (hour == 0 && minute == 0 && second == 0)
    clearAllClock();
    if (second >= 600) // 60s = 1 minute
    {
        second = 0;
        minute++;
        if (minute >= 60)
        {
            minute = 0;
            hour++;
            if (hour >= 24)
            {
                hour = 0;
            }
        }
    }

    led7_SetDigit(hour / 10, 0, 0);
    led7_SetDigit(hour % 10, 1, 0);
    led7_SetDigit(minute / 10, 2, 0);
    led7_SetDigit(minute % 10, 3, 0);

    second++; // every 100ms
}

void ex_5_chapter_2_handle()
{
    static uint8_t hour = 10;
    static uint8_t minute = 15;
    static uint8_t second = 0;
    static uint8_t shift_counter = 0;  // Đếm để tạo hiệu ứng shift
    static uint8_t display_buffer[4] = {1, 0, 1, 5};  // Buffer hiển thị ban đầu
    
    // Cập nhật thời gian
    if (second >= 600) // 60s = 1 minute
    {
        second = 0;
        minute++;
        if (minute >= 60)
        {
            minute = 0;
            hour++;
            if (hour >= 24)
            {
                hour = 0;
            }
        }
        
        // Cập nhật buffer với thời gian mới
        display_buffer[0] = hour / 10;
        display_buffer[1] = hour % 10;
        display_buffer[2] = minute / 10;
        display_buffer[3] = minute % 10;
    }
    
    // Tạo hiệu ứng shift qua phải mỗi 1 giây (10 lần gọi hàm)
    shift_counter++;
    if (shift_counter >= 10) // 1 giây = 10 x 100ms
    {
        shift_counter = 0;
        
        // Dịch các số qua phải
        uint8_t temp = display_buffer[3];  // Lưu số cuối
        display_buffer[3] = display_buffer[2];
        display_buffer[2] = display_buffer[1];
        display_buffer[1] = display_buffer[0];
        display_buffer[0] = temp;  // Số cuối chuyển lên đầu
    }
    
    // Hiển thị buffer
    led7_SetDigit(display_buffer[0], 0, 0);
    led7_SetDigit(display_buffer[1], 1, 0);
    led7_SetDigit(display_buffer[2], 2, 0);
    led7_SetDigit(display_buffer[3], 3, 0);

    second++; // every 100ms
}




/*
 * light_control.c
 *
 *  Created on: Nov 15, 2023
 *      Author: phamv
 */
#include "light_control.h"
#include "main.h"
uint8_t light_status = 0;

void lightProcess(){
	if(button_count[13] == 1){
		light_status = 1 - light_status;
		if(light_status == 1){
			uart_EspSendBytes("o", 1);
		} else {
			uart_EspSendBytes("o", 1);
		}
	}
	if(light_status == 1){
		HAL_GPIO_WritePin(OUTPUT_Y0_GPIO_Port, OUTPUT_Y0_Pin, 1);
	} else {
		HAL_GPIO_WritePin(OUTPUT_Y0_GPIO_Port, OUTPUT_Y0_Pin, 0);
	}
}

uint8_t count_led_debug = 0;

void test_Esp(){
	if(is_esp_connected == 1) return;

	if(uart_EspCheck() == 0) {
		uart_EspSendBytes("o", 1);

		static uint8_t dot_count = 0;
		dot_count = (dot_count + 1) % 40;

		if(dot_count == 0) lcd_ShowStr(10, 100, "Connecting   ", RED, WHITE, 24, 0);
		if(dot_count == 10) lcd_ShowStr(10, 100, "Connecting.  ", RED, WHITE, 24, 0);
		if(dot_count == 20) lcd_ShowStr(10, 100, "Connecting.. ", RED, WHITE, 24, 0);
		if(dot_count == 30) lcd_ShowStr(10, 100, "Connecting...", RED, WHITE, 24, 0);

		// Blink LED debug
		count_led_debug = (count_led_debug + 1) % 10;
		if(count_led_debug == 0){
			HAL_GPIO_TogglePin(DEBUG_LED_GPIO_Port, DEBUG_LED_Pin);
		}
	}
	else{
		is_esp_connected = 1;
		lcd_Clear(WHITE);

		lcd_Fill(0, 0, 240, 30, BLUE);
		lcd_ShowStr(60, 5, "MONITOR SYSTEM", WHITE, BLUE, 24, 0);

		lcd_ShowStr(10, 40, "Status:", BLACK, WHITE, 24, 0);
		lcd_ShowStr(100, 40, "ONLINE", GREEN, WHITE, 24, 0);

		lcd_DrawRectangle(10, 80, 230, 150, BLACK);
		lcd_ShowStr(20, 90, "Temperature:", RED, WHITE, 24, 0);


//		lcd_ShowStr(10, 50, "ESP Connect", GREEN, BLACK, 24, 0);

	}

}

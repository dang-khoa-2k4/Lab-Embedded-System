/*
 * lab6.c
 *
 *  Created on: Nov 17, 2025
 *      Author: hoquo
 */

#include "lab6.h"

static Monitor monitor;
static int thresshold = 70;
static int light_thresshold = 90;
static uint8_t trigger = 0;
static uint8_t colon_animation = 0;
static const int graph_width = 210,
			 graph_height = 130;
static int data_x[5] = {2, 0.2 * graph_width, 0.4 * graph_width, 0.6 * graph_width, 0.8 * graph_width};
static int data_y[5] = {0};


void systemInit(void)
{
	//Timer Tasks
	timerInit();

	buzzer_init();

	uart_init_rs232();

	ds3231_init();
	setTime();

	SCH_Init();

	// Sensor Tasks
	sensor_init();

	led_7seg_init();

	// LCD Tasks
	lcd_init();
	lcd_clear(WHITE);
}

void systemRun(void)
{
	SCH_Add_Task(updateSensorValue, 0, 100);
	SCH_Add_Task(displayLCD, 0, 250);
	SCH_Add_Task(alertBuzzer, 0, 100);
	SCH_Add_Task(updateTime, 0, 2);
	SCH_Add_Task(drawGraph, 0, 1000);
	while (1)
	{
		SCH_Dispatcher();
	}
}

void updateSensorValue(void)
{
	sensor_read();
	monitor.power = sensor_get_current() * sensor_get_voltage();
	monitor.light_emission = sensor_get_light();
	monitor.temp = sensor_get_temperature();
	monitor.humid = sensor_get_potentiometer() / 4095.0 * 100;
}

void alertBuzzer(void)
{
		if (monitor.humid > thresshold)
		{
			if (!timerIsSet(2))		timerSet(2, 1000);
			if (timerTimeout(2))
			{
				trigger = !trigger;
				timerReset(2);
				buzzer_set_volume(10 * trigger);
				if (trigger)
				{
					uart_rs232_send_string("Capture high mumidity: ");
					uart_rs232_send_num(monitor.humid);
					uart_rs232_send_string("\n");
				}
				timerSet(2, 1000);

			}
		}
		else
		{
			buzzer_set_volume(0);
			if (timerIsSet(2))	timerReset(2);
		}
}

void setTime(void)
{
	ds3231_write(ADDRESS_YEAR, 25);
	ds3231_write(ADDRESS_MONTH, 11);
	ds3231_write(ADDRESS_DATE, 17);
	ds3231_write(ADDRESS_DAY, 2);
	ds3231_write(ADDRESS_HOUR, 17);
	ds3231_write(ADDRESS_MIN, 3);
	ds3231_write(ADDRESS_SEC, 00);
}

void updateTime(void)
{
	if(!timerIsSet(3))	timerSet(3, 1000);
	if(!timerIsSet(4))	timerSet(4, 500);

	if (timerTimeout(3))
	{
		timerReset(3);
		ds3231_read_time();

		led_7seg_set_digit(ds3231_hours / 10, 0, 0);
		led_7seg_set_digit(ds3231_hours % 10, 1, 0);
		led_7seg_set_digit(ds3231_min / 10, 2, 0);
		led_7seg_set_digit(ds3231_min % 10, 3, 0);
		timerSet(3, 1000);
	}
	if (timerTimeout(4))
	{
		timerReset(4);
		colon_animation = !colon_animation;
		led_7seg_set_colon(colon_animation);
		timerSet(4, 500);
	}
	led_7seg_display();
}


uint32_t timestamp = 0;
void drawGraph(void)
{
	if (HAL_GetTick() - timestamp > 15000)
	{
		for (int i = 0; i < 4; i++)
		{
			data_y[i] = data_y[i+1];
		}
		data_y[4] = monitor.power / 500.0 * graph_height;
		timestamp = HAL_GetTick();
	}

	lcd_draw_line_chart(20, 170, graph_width, graph_height, "Power Graph", BLACK, BLUE, RED, data_x, data_y);
}

void displayLCD(void)
{
	lcd_show_string(30, 60, "Power:", RED, WHITE, 16, 1);
	lcd_show_float_num(100, 60, monitor.power, 7, BLACK, WHITE, 16);
	lcd_show_string(30, 92, "Light:", GREEN, WHITE, 16, 1);
	lcd_show_string(100, 92, (monitor.light_emission <= light_thresshold)? "Strong" : "Weak   ", BLACK, WHITE, 16, 0);
	lcd_show_string(30, 124, "Temp:", BROWN, WHITE, 16, 1);
	lcd_show_float_num(100, 124, monitor.temp, 4, BLACK, WHITE, 16);
	lcd_show_string(30, 156, "Humid:", BLUE, WHITE, 16, 1);
	lcd_show_int_num(100, 156, monitor.humid, 3, BLACK, WHITE, 16);

}

void HAL_TIM_PeriodElapsedCallback(TIM_HandleTypeDef *htim)
{
	if (htim->Instance == TIM2) {
		SCH_Update();
	}
	else if (htim->Instance == TIM4) {
		timerRun();
	}
}



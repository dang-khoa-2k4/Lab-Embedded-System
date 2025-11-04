#include "clock.h"
#include "lcd.h"
#include "button.h"
#include "ds3231.h"
#include "uart.h"
#include "software_timer.h"
#include <stdio.h>
#include <string.h>
#include <stdlib.h>

// Private variables
static ClockMode current_mode = MODE_VIEW;
static EditParam current_param = PARAM_HOUR;

// Time storage
static uint8_t temp_time[PARAM_COUNT] = {0};
static uint8_t alarm_time[PARAM_COUNT] = {0};
static uint8_t alarm_enabled = 0;
static uint8_t alarm_triggered = 0;

// Display state
static uint8_t blink_state = 0;

// UART communication
static uint8_t ring_buffer[RING_BUFFER_SIZE];
static volatile uint16_t ring_head = 0;
static volatile uint16_t ring_tail = 0;
static volatile uint8_t data_available = 0;
static uint8_t uart_rx_byte;

// Update mode state
static uint8_t update_param = 0;
static uint8_t retry_count = 0;
static uint8_t waiting_response = 0;
static uint16_t timeout_counter = 0;
static char response_buffer[20];
static uint8_t response_index = 0;

// Parameter information
static const char* param_names[] = {"Hour", "Min", "Sec", "Day", "Date", "Month", "Year"};
static const uint8_t param_max[] = {23, 59, 59, 7, 31, 12, 99};
static const uint8_t param_min[] = {0, 0, 0, 1, 1, 1, 0};

// Private function prototypes
static void handle_buttons(void);
static void handle_update_mode(void);
static void send_uart_request(void);
static void process_uart_response(void);
static uint8_t validate_parameter(uint8_t param, uint8_t value);
static void ring_buffer_put(uint8_t data);
static uint8_t ring_buffer_get(uint8_t* data);
static void copy_current_time_to_temp(void);
static void write_temp_to_rtc(void);
static void check_alarm(void);
static void display_clock(void);
static void display_edit_mode(void);
static void display_alarm_mode(void);
static void display_update_mode(void);

// Implementation của các hàm từ Lab5.c
[Copy toàn bộ implementation của các hàm từ Lab5.c vào đây]
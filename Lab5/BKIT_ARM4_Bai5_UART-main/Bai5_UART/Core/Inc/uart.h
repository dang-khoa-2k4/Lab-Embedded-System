/*
 * uart.h
 *
 *  Created on: Sep 26, 2023
 *      Author: HaHuyen
 */
#ifndef INC_UART_H_
#define INC_UART_H_

#include "usart.h"
#include <stdio.h>
#include "utils.h"

// UART Configuration
#define UART_BAUDRATE    9600
#define UART_BUFFER_SIZE 128

// Function prototypes
void uart_init_rs232(void);
void uart_Rs232SendString(const char* str);  // Changed to const char*
void uart_Rs232SendBytes(uint8_t* bytes, uint16_t size);
void uart_Rs232SendNum(uint32_t num);
void uart_Rs232SendNumPercent(uint32_t num);

#endif /* INC_UART_H_ */


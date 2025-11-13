/*
 * uart.c
 *
 *  Created on: Sep 26, 2023
 *      Author: HaHuyen
 */
#include "uart.h"
#include "ring_buffer.h"

uint8_t receive_buffer1 = 0;
uint8_t msg[100];
RingBuffer rs232_rx_buffer;

uint8_t uart_data_flag = 0;

void uart_init_rs232(){
    // init ring buffer
    ringBuffer_Init(&rs232_rx_buffer);

	HAL_UART_Receive_IT(&huart1, &receive_buffer1, 1);
}

void uart_Rs232SendString(uint8_t* str){
	HAL_UART_Transmit(&huart1, (void*)msg, sprintf((void*)msg,"%s",str), 10);
}

void uart_Rs232SendBytes(uint8_t* bytes, uint16_t size){
	HAL_UART_Transmit(&huart1, bytes, size, 10);
}

void uart_Rs232SendNum(uint32_t num){
	if(num == 0){
		uart_Rs232SendString("0");
		return;
	}
    uint8_t num_flag = 0;
    int i;
	if(num < 0) uart_Rs232SendString("-");
    for(i = 10; i > 0; i--)
    {
        if((num / mypow(10, i-1)) != 0)
        {
            num_flag = 1;
            sprintf((void*)msg,"%d",num/mypow(10, i-1));
            uart_Rs232SendString(msg);
        }
        else
        {
            if(num_flag != 0)
            	uart_Rs232SendString("0");
        }
        num %= mypow(10, i-1);
    }
}

void uart_Rs232SendNumPercent(uint32_t num)
{
	sprintf((void*)msg,"%ld",num/100);
    uart_Rs232SendString(msg);
    uart_Rs232SendString(".");
    sprintf((void*)msg,"%ld",num%100);
    uart_Rs232SendString(msg);
}

void HAL_UART_RxCpltCallback(UART_HandleTypeDef *huart){
	if(huart->Instance == USART1){
		// rs232 isr
		// can be modified
		HAL_UART_Transmit(&huart1, &receive_buffer1, 1, 10);
        ringBuffer_Write(&rs232_rx_buffer, receive_buffer1);

        // set flag to process in main loop if needed
        uart_data_flag = 1;

		// turn on the receice interrupt
		HAL_UART_Receive_IT(&huart1, &receive_buffer1, 1);
	}
}

void uart_ProcessReceivedData(void){
    uint8_t received_data;
    while(ringBuffer_Available(&rs232_rx_buffer) > 0){
        if(ringBuffer_Read(&rs232_rx_buffer, &received_data)){
            // Xử lý dữ liệu nhận được
            // Ví dụ: echo lại dữ liệu
            uart_Rs232SendBytes(&received_data, 1);

            // Hoặc xử lý theo yêu cầu của bạn
            // Ví dụ: kiểm tra lệnh, phân tích chuỗi, v.v.
        }
    }
}

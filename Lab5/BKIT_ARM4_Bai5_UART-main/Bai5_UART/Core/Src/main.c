/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file           : main.c
  * @brief          : Main program body
  ******************************************************************************
  * @attention
  *
  * <h2><center>&copy; Copyright (c) 2023 STMicroelectronics.
  * All rights reserved.</center></h2>
  *
  * This software component is licensed by ST under BSD 3-Clause license,
  * the "License"; You may not use this file except in compliance with the
  * License. You may obtain a copy of the License at:
  *                        opensource.org/licenses/BSD-3-Clause
  *
  ******************************************************************************
  */
/* USER CODE END Header */
/* Includes ------------------------------------------------------------------*/
#include "main.h"
#include "i2c.h"
#include "spi.h"
#include "tim.h"
#include "usart.h"
#include "gpio.h"
#include "fsmc.h"

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */
#include "software_timer.h"
#include "led_7seg.h"
#include "button.h"
#include "lcd.h"
#include "picture.h"
#include "ds3231.h"
#include "uart.h"
#include "clock.h"
#include "ring_buffer.h"
#include <stdio.h>
#include <string.h>
/* USER CODE END Includes */

/* Private typedef -----------------------------------------------------------*/
/* USER CODE BEGIN PTD */

/* USER CODE END PTD */

/* Private defines -----------------------------------------------------------*/
/* USER CODE BEGIN PD */
#define RING_BUFFER_SIZE 128   // Size of UART ring buffer
#define TIMEOUT_MS      10000  // 10 second timeout
#define MAX_RETRY       3      // Maximum retry attempts
#define PARAM_COUNT     7      // Number of time parameters to update
/* USER CODE END PD */

/* Private macro -------------------------------------------------------------*/
/* USER CODE BEGIN PM */

/* USER CODE END PM */

/* Private variables ---------------------------------------------------------*/

/* USER CODE BEGIN PV */
// UART and buffer variables
ring_buffer_t uart_rx_buffer;
uint8_t uart_rx_byte;
char response_buffer[20];
uint8_t response_index = 0;
uint8_t waiting_response = 0;
uint16_t timeout_counter = 0;
uint8_t retry_count = 0;

// UPDATE mode parameters
uint8_t update_param = 0;
const uint8_t param_min[] = {0, 0, 0, 1, 1, 1, 0};  // Min values for each parameter
const uint8_t param_max[] = {23, 59, 59, 7, 31, 12, 99};  // Max values
const char* param_names[] = {"Hour", "Minute", "Second", "Day", "Date", "Month", "Year"};
/* USER CODE END PV */

/* Private function prototypes -----------------------------------------------*/
void SystemClock_Config(void);
/* USER CODE BEGIN PFP */
void system_init();
void test_LedDebug();
void test_Uart();
/* USER CODE END PFP */

/* Private user code ---------------------------------------------------------*/
/* USER CODE BEGIN 0 */

/* USER CODE END 0 */

/**
  * @brief  The application entry point.
  * @retval int
  */
int main(void)
{
  /* USER CODE BEGIN 1 */

  /* USER CODE END 1 */

  /* MCU Configuration--------------------------------------------------------*/

  /* Reset of all peripherals, Initializes the Flash interface and the Systick. */
  HAL_Init();

  /* USER CODE BEGIN Init */

  /* USER CODE END Init */

  /* Configure the system clock */
  SystemClock_Config();

  /* USER CODE BEGIN SysInit */

  /* USER CODE END SysInit */

  /* Initialize all configured peripherals */
  MX_GPIO_Init();
  MX_TIM2_Init();
  MX_SPI1_Init();
  MX_FSMC_Init();
  MX_I2C1_Init();
  MX_USART1_UART_Init();
  /* USER CODE BEGIN 2 */
  system_init();
  /* USER CODE END 2 */

  /* Infinite loop */
  /* USER CODE BEGIN WHILE */
  while (1)
  {
    /* USER CODE END WHILE */
    while (!flag_timer2);
    flag_timer2 = 0;
    
    button_Scan();
    clock_handle_buttons();
    ds3231_ReadTime();
    clock_display();
    
    test_LedDebug(); // Keep for debug LED
    /* USER CODE BEGIN 3 */
  }
  /* USER CODE END 3 */
}

/**
  * @brief System Clock Configuration
  * @retval None
  */
void SystemClock_Config(void)
{
  RCC_OscInitTypeDef RCC_OscInitStruct = {0};
  RCC_ClkInitTypeDef RCC_ClkInitStruct = {0};

  /** Configure the main internal regulator output voltage
  */
  __HAL_RCC_PWR_CLK_ENABLE();
  __HAL_PWR_VOLTAGESCALING_CONFIG(PWR_REGULATOR_VOLTAGE_SCALE1);
  /** Initializes the RCC Oscillators according to the specified parameters
  * in the RCC_OscInitTypeDef structure.
  */
  RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_HSI;
  RCC_OscInitStruct.HSIState = RCC_HSI_ON;
  RCC_OscInitStruct.HSICalibrationValue = RCC_HSICALIBRATION_DEFAULT;
  RCC_OscInitStruct.PLL.PLLState = RCC_PLL_ON;
  RCC_OscInitStruct.PLL.PLLSource = RCC_PLLSOURCE_HSI;
  RCC_OscInitStruct.PLL.PLLM = 8;
  RCC_OscInitStruct.PLL.PLLN = 168;
  RCC_OscInitStruct.PLL.PLLP = RCC_PLLP_DIV2;
  RCC_OscInitStruct.PLL.PLLQ = 4;
  if (HAL_RCC_OscConfig(&RCC_OscInitStruct) != HAL_OK)
  {
    Error_Handler();
  }
  /** Initializes the CPU, AHB and APB buses clocks
  */
  RCC_ClkInitStruct.ClockType = RCC_CLOCKTYPE_HCLK|RCC_CLOCKTYPE_SYSCLK
                              |RCC_CLOCKTYPE_PCLK1|RCC_CLOCKTYPE_PCLK2;
  RCC_ClkInitStruct.SYSCLKSource = RCC_SYSCLKSOURCE_PLLCLK;
  RCC_ClkInitStruct.AHBCLKDivider = RCC_SYSCLK_DIV1;
  RCC_ClkInitStruct.APB1CLKDivider = RCC_HCLK_DIV4;
  RCC_ClkInitStruct.APB2CLKDivider = RCC_HCLK_DIV4;

  if (HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_5) != HAL_OK)
  {
    Error_Handler();
  }
}

/* USER CODE BEGIN 4 */
void system_init(){
	  HAL_GPIO_WritePin(OUTPUT_Y0_GPIO_Port, OUTPUT_Y0_Pin, 0);
	  HAL_GPIO_WritePin(OUTPUT_Y1_GPIO_Port, OUTPUT_Y1_Pin, 0);
	  HAL_GPIO_WritePin(DEBUG_LED_GPIO_Port, DEBUG_LED_Pin, 0);
	  
	  timer_init();
	  led7_init();
	  button_init();
	  lcd_init();
	  ds3231_init();
	  uart_init_rs232();
	  clock_init();
	  
	  // Initialize ring buffer
	  ring_buffer_init(&uart_rx_buffer);
	  
	  // Start UART receive
	  HAL_UART_Receive_IT(&huart1, &uart_rx_byte, 1);
	  
	  // Start timers
	  setTimer2(50);   // Main loop
	  setTimer3(500);  // Blink
	  setTimer4(100);  // Timeout check
	  
	  lcd_Clear(BLACK);
	  lcd_Fill(0, 0, 240, 30, BLUE);
	  lcd_StrCenter(0, 5, "CLOCK + UART", WHITE, BLUE, 16, 1);
	  
	  uart_Rs232SendString("System Ready\r\n");
}

uint16_t count_led_debug = 0;

void test_LedDebug(){
	count_led_debug = (count_led_debug + 1)%20;
	if(count_led_debug == 0){
		HAL_GPIO_TogglePin(DEBUG_LED_GPIO_Port, DEBUG_LED_Pin);
	}
}

void test_button(){
	for(int i = 0; i < 16; i++){
		if(button_count[i] == 1){
			led7_SetDigit(i/10, 2, 0);
			led7_SetDigit(i%10, 3, 0);
		}
	}
}

void HAL_UART_RxCpltCallback(UART_HandleTypeDef *huart) {
    if (huart->Instance == USART1) {
        ring_buffer_put(&uart_rx_buffer, uart_rx_byte);
        // Restart UART receive immediately
        HAL_UART_Receive_IT(&huart1, &uart_rx_byte, 1);
    }
}

void display_update_mode(void) {
    // Clear previous display
    lcd_Fill(0, 40, 240, 280, BLACK);
    
    // Display current mode
    lcd_Fill(0, 40, 240, 70, BLUE);
    lcd_StrCenter(0, 45, "UPDATE MODE", WHITE, BLUE, 16, 1);
    
    // Show updating parameter
    char updating_str[30];
    sprintf(updating_str, "Updating %s...", param_names[update_param]);
    lcd_StrCenter(0, 80, updating_str, WHITE, BLACK, 16, 1);
    
    // Display current parameter
    char param_str[30];
    sprintf(param_str, "Enter %s: (%d-%d)", 
            param_names[update_param],
            param_min[update_param],
            param_max[update_param]);
    lcd_StrCenter(0, 100, param_str, WHITE, BLACK, 16, 1);
    
    // Display retry count if any
    if (retry_count > 0) {
        char retry_str[20];
        sprintf(retry_str, "Retry: %d/%d", retry_count, MAX_RETRY);
        lcd_StrCenter(0, 150, retry_str, YELLOW, BLACK, 16, 1);
    }
    
    // Display timeout progress if waiting
    if (waiting_response) {
        char timeout_str[20];
        sprintf(timeout_str, "Timeout: %d%%", 
                (timeout_counter * 100) / (TIMEOUT_MS / 100));
        lcd_StrCenter(0, 200, timeout_str, 
                     timeout_counter > (TIMEOUT_MS / 200) ? RED : GREEN, 
                     BLACK, 16, 1);
    }
}

void handle_update_mode(void) {
    // Send request if not waiting
    if (!waiting_response) {
        char request[50];
        sprintf(request, "Enter %s (%d-%d): ", 
                param_names[update_param], 
                param_min[update_param], 
                param_max[update_param]);
        uart_Rs232SendString(request);
        
        waiting_response = 1;
        timeout_counter = 0;
        response_index = 0;
        memset(response_buffer, 0, sizeof(response_buffer));
        setTimer4(100);  // Start timeout timer
    }
    
    // Process received data
    uint8_t byte;
    while (ring_buffer_get(&uart_rx_buffer, &byte)) {
        if (byte == '\n' || byte == '\r') {
            // End of response - only process if we have data
            if (response_index > 0) {
                response_buffer[response_index] = '\0';
                int value = atoi(response_buffer);
                
                // Validate
                if (value >= param_min[update_param] && value <= param_max[update_param]) {
                    char log_msg[50];
                    sprintf(log_msg, "OK! %s = %d\r\n", param_names[update_param], value);
                    uart_Rs232SendString(log_msg);
                    
                    // Store value and move to next parameter
                    switch(update_param) {
                        case 0: ds3231_Write(ADDRESS_HOUR, value); break;
                        case 1: ds3231_Write(ADDRESS_MIN, value); break;
                        case 2: ds3231_Write(ADDRESS_SEC, value); break;
                        case 3: ds3231_Write(ADDRESS_DAY, value); break;
                        case 4: ds3231_Write(ADDRESS_DATE, value); break;
                        case 5: ds3231_Write(ADDRESS_MONTH, value); break;
                        case 6: ds3231_Write(ADDRESS_YEAR, value); break;
                    }
                    
                    update_param++;
                    retry_count = 0;
                    
                    if (update_param >= PARAM_COUNT) {
                        uart_Rs232SendString("\r\n=== Time updated successfully! ===\r\n");
                        lcd_Fill(40, 250, 200, 280, GREEN);
                        lcd_StrCenter(0, 255, "SUCCESS!", WHITE, GREEN, 16, 1);
                        HAL_Delay(1500);
                        current_mode = MODE_VIEW;
                    }
                    waiting_response = 0;
                } else {
                    char error_msg[60];
                    sprintf(error_msg, "Invalid! Range: %d-%d\r\n", 
                            param_min[update_param], param_max[update_param]);
                    uart_Rs232SendString(error_msg);
                    
                    retry_count++;
                    if (retry_count >= MAX_RETRY) {
                        uart_Rs232SendString("ERROR: Max retries reached!\r\n");
                        lcd_Fill(40, 250, 200, 280, RED);
                        lcd_StrCenter(0, 255, "INVALID DATA", WHITE, RED, 16, 1);
                        HAL_Delay(2000);
                        current_mode = MODE_VIEW;
                    } else {
                        waiting_response = 0;
                    }
                }
            }
            response_index = 0;
            memset(response_buffer, 0, sizeof(response_buffer));
        }
        else if (response_index < sizeof(response_buffer) - 1) {
            // Only accept digits
            if (byte >= '0' && byte <= '9') {
                response_buffer[response_index++] = byte;
            }
        }
    }
    
    // Check timeout
    if (flag_timer4) {
        flag_timer4 = 0;
        timeout_counter++;
        
        if (timeout_counter >= (TIMEOUT_MS / 100)) {
            retry_count++;
            
            char retry_msg[50];
            sprintf(retry_msg, "Timeout! Retry %d/%d\r\n", retry_count, MAX_RETRY);
            uart_Rs232SendString(retry_msg);
            
            if (retry_count >= MAX_RETRY) {
                lcd_Fill(40, 250, 200, 280, RED);
                lcd_StrCenter(0, 255, "TIMEOUT ERROR", WHITE, RED, 16, 1);
                uart_Rs232SendString("ERROR: Max retries reached!\r\n");
                HAL_Delay(2000);
                current_mode = MODE_VIEW;
            } else {
                waiting_response = 0;
                timeout_counter = 0;
            }
        }
    }
}

void test_Uart(){
	if(button_count[12] == 1){
		uart_Rs232SendNum(ds3231_hours);
		uart_Rs232SendString(":");
		uart_Rs232SendNum(ds3231_min);
		uart_Rs232SendString(":");
		uart_Rs232SendNum(ds3231_sec);
		uart_Rs232SendString("\n");
	}
}
/* USER CODE END 4 */

/**
  * @brief  This function is executed in case of error occurrence.
  * @retval None
  */
void Error_Handler(void)
{
  /* USER CODE BEGIN Error_Handler_Debug */
  /* User can add his own implementation to report the HAL error return state */
  __disable_irq();
  while (1)
  {
  }
  /* USER CODE END Error_Handler_Debug */
}

#ifdef  USE_FULL_ASSERT
/**
  * @brief  Reports the name of the source file and the source line number
  *         where the assert_param error has occurred.
  * @param  file: pointer to the source file name
  * @param  line: assert_param error line source number
  * @retval None
  */
void assert_failed(uint8_t *file, uint32_t line)
{
  /* USER CODE BEGIN 6 */
  /* User can add his own implementation to report the file name and line number,
     ex: printf("Wrong parameters value: file %s on line %d\r\n", file, line) */
  /* USER CODE END 6 */
}
#endif /* USE_FULL_ASSERT */

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/

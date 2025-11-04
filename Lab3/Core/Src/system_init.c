#include "system_init.h"
#include "global.h"
#include "software_timer.h"
#include "button.h"
#include "led_7seg.h"
#include "led_display.h"
#include "fsm_automatic.h"
#include "fsm_manual.h"
#include "time_config.h"
#include "error.h"
#include "main.h"

/* Initialization Status */
uint8_t init_complete = 0;

/**
  * @brief  Initialize peripherals (GPIO, SPI, FSMC, TIM)
  */
ErrorCode peripheral_init(void) {
    // Set GPIO outputs to safe state
    HAL_GPIO_WritePin(OUTPUT_Y0_GPIO_Port, OUTPUT_Y0_Pin, GPIO_PIN_RESET);
    HAL_GPIO_WritePin(OUTPUT_Y1_GPIO_Port, OUTPUT_Y1_Pin, GPIO_PIN_RESET);
    HAL_GPIO_WritePin(DEBUG_LED_GPIO_Port, DEBUG_LED_Pin, GPIO_PIN_RESET);
    
    return ERROR_NONE;
}

/**
  * @brief  Initialize software timer system
  */
ErrorCode timer_system_init(void) {
    timer_init();
    
    // Start timers with configured intervals
    setTimer2(BUTTON_SCAN_INTERVAL);
    setTimer3(BLINK_INTERVAL);
    
    return ERROR_NONE;
}

/**
  * @brief  Initialize button module
  */
ErrorCode button_system_init(void) {
    button_init();
    return ERROR_NONE;
}

/**
  * @brief  Initialize display
  */
ErrorCode display_system_init(void) {
    led7_init();
    display_init();
    return ERROR_NONE;
}

/**
  * @brief  Initialize FSM
  */
ErrorCode fsm_system_init(void) {
    // Initialize global variables
    current_mode = MODE_NORMAL;
    traffic_state = STATE_GREEN1_RED2;
    state_counter = 0;
    blink_state = 0;
    
    // Set default durations
    red_duration = DEFAULT_RED_DURATION;
    green_duration = DEFAULT_GREEN_DURATION;
    yellow_duration = DEFAULT_YELLOW_DURATION;
    temp_duration = 0;
    
    // Initialize FSM modules
    fsm_automatic_init();
    fsm_manual_init();
    
    return ERROR_NONE;
}

/**
  * @brief  Initialize entire system
  */
ErrorCode system_init(void) {
    ErrorCode err;
    
    // Initialize in sequence
    err = peripheral_init();
    if (err != ERROR_NONE) return err;
    
    err = timer_system_init();
    if (err != ERROR_NONE) return err;
    
    err = button_system_init();
    if (err != ERROR_NONE) return err;
    
    err = display_system_init();
    if (err != ERROR_NONE) return err;
    
    err = fsm_system_init();
    if (err != ERROR_NONE) return err;
    
    init_complete = 1;
    return ERROR_NONE;
}

/**
  * @brief  Check if system is initialized
  */
int is_system_initialized(void) {
    return init_complete;
}
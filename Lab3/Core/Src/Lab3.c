/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file           : main.c
  * @brief          : Lab 3 - Traffic Light System with FSM
  * @author         : NgoDucAnh and DangKhoa
  ******************************************************************************
  */
/* USER CODE END Header */

/* Includes ------------------------------------------------------------------*/
#include "main.h"
#include "spi.h"
#include "tim.h"
#include "gpio.h"
#include "fsmc.h"

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */
#include "software_timer.h"
#include "led_7seg.h"
#include "button.h"
#include "lcd.h"
#include "picture.h"
#include <stdio.h>
/* USER CODE END Includes */

/* Private typedef -----------------------------------------------------------*/
/* USER CODE BEGIN PTD */
typedef enum {
    MODE_NORMAL = 0,
    MODE_MOD_RED,
    MODE_MOD_GREEN,
    MODE_MOD_YELLOW
} SystemMode;

typedef enum {
    STATE_GREEN1_RED2 = 0,  // Road 1: Green, Road 2: Red
    STATE_YELLOW1_RED2,      // Road 1: Yellow, Road 2: Red
    STATE_RED1_GREEN2,       // Road 1: Red, Road 2: Green
    STATE_RED1_YELLOW2,      // Road 1: Red, Road 2: Yellow
    STATE_RED1_RED2          // Emergency state: All Red
} TrafficState;
/* USER CODE END PTD */

/* Private define ------------------------------------------------------------*/
/* USER CODE BEGIN PD */
#define BTN_MODE    0   // Button 0: Switch mode
#define BTN_INC     1   // Button 1: Increase value
#define BTN_CONFIRM 2   // Button 2: Confirm

#define TIMER_CYCLE   1  // Timer for traffic light cycle (1s)
#define TIMER_SCAN    2  // Timer for button scan (50ms)
#define TIMER_BLINK   3  // Timer for LED blink (500ms = 2Hz)
/* USER CODE END PD */

/* Private variables ---------------------------------------------------------*/
/* USER CODE BEGIN PV */
// System state
SystemMode current_mode = MODE_NORMAL;
TrafficState traffic_state = STATE_GREEN1_RED2;

// Cycle times (in seconds)
uint8_t red_duration = 10;
uint8_t green_duration = 8;
uint8_t yellow_duration = 2;

// Modification mode
uint8_t temp_duration = 0;
uint8_t blink_state = 0;  // For 2Hz blinking

// State countdown timer
uint16_t state_counter = 0;  // Counts in 50ms
/* USER CODE END PV */

/* Private function prototypes -----------------------------------------------*/
void SystemClock_Config(void);
/* USER CODE BEGIN PFP */
void system_init(void);
void draw_traffic_lights(void);
void update_traffic_fsm(void);
void handle_button_mode(void);
void handle_button_increase(void);
void handle_button_confirm(void);
void draw_mode_info(void);
uint8_t get_current_state_duration(void);
/* USER CODE END PFP */

int main(void)
{
  /* MCU Configuration--------------------------------------------------------*/
  HAL_Init();
  SystemClock_Config();
  
  /* Initialize all configured peripherals */
  MX_GPIO_Init();
  MX_FSMC_Init();
  MX_SPI1_Init();
  MX_TIM2_Init();

  /* USER CODE BEGIN 2 */
  system_init();
  lcd_Clear(BLACK);
  
  // Display initial screen
  lcd_Fill(0, 0, 240, 30, BLUE);
  lcd_StrCenter(0, 5, "TRAFFIC LIGHT SYSTEM", WHITE, BLUE, 16, 1);
  
  // Start timers
  setTimer2(50);   // Button scan every 50ms
  setTimer3(500);  // Blink every 500ms (2Hz)
  
  state_counter = 0;
  /* USER CODE END 2 */

  /* Infinite loop */
  while (1)
  {
    /* USER CODE BEGIN WHILE */
    // Wait for 50ms tick
    while (!flag_timer2);
    flag_timer2 = 0;
    
    // Scan buttons (debouncing handled by button library)
    button_Scan();
    
    // Handle button presses
    handle_button_mode();
    handle_button_increase();
    handle_button_confirm();
    
    // Update FSM
    if (current_mode == MODE_NORMAL) {
      update_traffic_fsm();
    }
    
    // Handle blink timer for modification modes
    if (flag_timer3) {
      flag_timer3 = 0;
      blink_state = !blink_state;
    }
    
    // Redraw display
    draw_traffic_lights();
    draw_mode_info();
    
    /* USER CODE END WHILE */
  }
}

/* USER CODE BEGIN 4 */

/**
  * @brief  System initialization
  */
void system_init(void)
{
  HAL_GPIO_WritePin(OUTPUT_Y0_GPIO_Port, OUTPUT_Y0_Pin, 0);
  HAL_GPIO_WritePin(OUTPUT_Y1_GPIO_Port, OUTPUT_Y1_Pin, 0);
  HAL_GPIO_WritePin(DEBUG_LED_GPIO_Port, DEBUG_LED_Pin, 0);
  
  timer_init();
  led7_init();
  button_init();
  lcd_init();
  
  // Initialize system state
  current_mode = MODE_NORMAL;
  traffic_state = STATE_GREEN1_RED2;
  state_counter = 0;
  blink_state = 0;
  
  // Initialize default durations
  red_duration = 10;
  green_duration = 8;
  yellow_duration = 2;
  temp_duration = 0;
}

/**
  * @brief  Get duration for current traffic state
  */
uint8_t get_current_state_duration(void)
{
  switch (traffic_state) {
    case STATE_GREEN1_RED2:
    case STATE_RED1_GREEN2:
      return green_duration;
    case STATE_YELLOW1_RED2:
    case STATE_RED1_YELLOW2:
      return yellow_duration;
    default:
      return 1;
  }
}

/**
  * @brief  Update traffic light FSM (called every 50ms in NORMAL mode)
  */
void update_traffic_fsm(void)
{
  state_counter++;
  
  // Check if current state duration elapsed (50ms ticks to seconds)
  uint16_t duration_in_ticks = get_current_state_duration() * 20; // 20 ticks = 1s
  
  if (state_counter >= duration_in_ticks) {
    state_counter = 0;
    
    // Transition to next state
    switch (traffic_state) {
      case STATE_GREEN1_RED2:
        traffic_state = STATE_YELLOW1_RED2;
        break;
      case STATE_YELLOW1_RED2:
        traffic_state = STATE_RED1_GREEN2;
        break;
      case STATE_RED1_GREEN2:
        traffic_state = STATE_RED1_YELLOW2;
        break;
      case STATE_RED1_YELLOW2:
        traffic_state = STATE_GREEN1_RED2;
        break;
      default:
        // Reset to safe state if invalid state detected
        traffic_state = STATE_RED1_RED2;
        state_counter = 0;
        break;
    }
    
    // Validate durations after state change
    if (red_duration < 1) red_duration = 1;
    if (green_duration < 1) green_duration = 1;
    if (yellow_duration < 1) yellow_duration = 1;
  }
}

/**
  * @brief  Handle mode button (Button 0)
  */
void handle_button_mode(void)
{
  if (button_count[BTN_MODE] == 1) {  // Short press detected
    // Cycle through modes
    current_mode = (current_mode + 1) % 4;
    
    // Initialize temp value when entering modification mode
    switch (current_mode) {
      case MODE_MOD_RED:
        temp_duration = red_duration;
        break;
      case MODE_MOD_GREEN:
        temp_duration = green_duration;
        break;
      case MODE_MOD_YELLOW:
        temp_duration = yellow_duration;
        break;
      case MODE_NORMAL:
        state_counter = 0;  // Reset counter
        break;
    }
  }
}

/**
  * @brief  Handle increase button (Button 1)
  */
void handle_button_increase(void)
{
  if (current_mode == MODE_NORMAL) return;
  
  // Short press: increase by 1 (detected on first press)
  if (button_count[BTN_INC] == 1) {
    temp_duration++;
    if (temp_duration > 99) temp_duration = 1;
    if (temp_duration < 1) temp_duration = 1;
  }
  // Long press: increase every 200ms (4 scans of 50ms)
  else if (button_count[BTN_INC] > 20 && button_count[BTN_INC] % 4 == 0) {
    temp_duration++;
    if (temp_duration > 99) temp_duration = 1;
    if (temp_duration < 1) temp_duration = 1;
  }
}

/**
  * @brief  Handle confirm button (Button 2)
  */
void handle_button_confirm(void)
{
  if (current_mode == MODE_NORMAL) return;
  
  if (button_count[BTN_CONFIRM] == 1) {  // Short press
    // Save the modified value
    switch (current_mode) {
      case MODE_MOD_RED:
        red_duration = temp_duration;
        break;
      case MODE_MOD_GREEN:
        green_duration = temp_duration;
        break;
      case MODE_MOD_YELLOW:
        yellow_duration = temp_duration;
        break;
      default:
        break;
    }
    
    // Return to normal mode
    current_mode = MODE_NORMAL;
    state_counter = 0;
  }
}

/**
  * @brief  Draw traffic lights on LCD
  */
void draw_traffic_lights(void)
{
  // Define positions for 6 lights (2 roads)
  // Road 1 (left): x=60, Red(y=80), Yellow(y=140), Green(y=200)
  // Road 2 (right): x=180, Red(y=80), Yellow(y=140), Green(y=200)
  
  uint16_t red1_color = BLACK, yellow1_color = BLACK, green1_color = BLACK;
  uint16_t red2_color = BLACK, yellow2_color = BLACK, green2_color = BLACK;
  
  if (current_mode == MODE_NORMAL) {
    // Determine which lights should be on based on state
    switch (traffic_state) {
      case STATE_GREEN1_RED2:
        green1_color = GREEN;
        red2_color = RED;
        break;
      case STATE_YELLOW1_RED2:
        yellow1_color = YELLOW;
        red2_color = RED;
        break;
      case STATE_RED1_GREEN2:
        red1_color = RED;
        green2_color = GREEN;
        break;
      case STATE_RED1_YELLOW2:
        red1_color = RED;
        yellow2_color = YELLOW;
        break;
      case STATE_RED1_RED2:
        red1_color = RED;
        red2_color = RED;
        break;
      default:
        // Default to all lights off for safety
        break;
    }
  } else {
    // Modification mode: blink the corresponding color
    uint16_t blink_color = blink_state ? WHITE : BLACK;
    
    switch (current_mode) {
      case MODE_MOD_RED:
        red1_color = blink_color;
        red2_color = blink_color;
        break;
      case MODE_MOD_GREEN:
        green1_color = blink_color;
        green2_color = blink_color;
        break;
      case MODE_MOD_YELLOW:
        yellow1_color = blink_color;
        yellow2_color = blink_color;
        break;
      default:
        break;
    }
  }
  
  // Draw Road 1 lights
  lcd_DrawCircle(60, 80, red1_color, 20, 1);
  lcd_DrawCircle(60, 140, yellow1_color, 20, 1);
  lcd_DrawCircle(60, 200, green1_color, 20, 1);
  
  // Draw Road 2 lights
  lcd_DrawCircle(180, 80, red2_color, 20, 1);
  lcd_DrawCircle(180, 140, yellow2_color, 20, 1);
  lcd_DrawCircle(180, 200, green2_color, 20, 1);
  
  // Draw road labels
  lcd_ShowStr(30, 230, "ROAD 1", WHITE, BLACK, 16, 0);
  lcd_ShowStr(150, 230, "ROAD 2", WHITE, BLACK, 16, 0);
}

/**
  * @brief  Draw mode information and countdown
  */
void draw_mode_info(void)
{
  char info_str[30];
  
  // Clear info area
  lcd_Fill(0, 250, 240, 320, BLACK);
  
  if (current_mode == MODE_NORMAL) {
    sprintf(info_str, "MODE: NORMAL");
    lcd_ShowStr(10, 255, info_str, CYAN, BLACK, 16, 0);
    
    // Show countdown for current state
    uint8_t duration = get_current_state_duration();
    uint16_t elapsed = state_counter / 20;  // Convert ticks to seconds
    uint16_t remaining = duration - elapsed;
    
    sprintf(info_str, "Time: %2d s", remaining);
    lcd_ShowStr(10, 275, info_str, WHITE, BLACK, 20, 0);
    
    // Show cycle times
    sprintf(info_str, "R:%d G:%d Y:%d", red_duration, green_duration, yellow_duration);
    lcd_ShowStr(10, 300, info_str, YELLOW, BLACK, 16, 0);
    
  } else {
    // Modification mode
    const char* mode_names[] = {"", "RED", "GREEN", "YELLOW"};
    sprintf(info_str, "MODIFY %s", mode_names[current_mode]);
    lcd_ShowStr(10, 255, info_str, MAGENTA, BLACK, 16, 0);
    
    sprintf(info_str, "Duration: %2d s", temp_duration);
    lcd_ShowStr(10, 275, info_str, WHITE, BLACK, 20, 0);
    
    lcd_ShowStr(10, 300, "BTN1:+ BTN2:OK", YELLOW, BLACK, 16, 0);
  }
}

/* USER CODE END 4 */

/**
  * @brief System Clock Configuration
  */
void SystemClock_Config(void)
{
  // ... (keep existing clock config)
}
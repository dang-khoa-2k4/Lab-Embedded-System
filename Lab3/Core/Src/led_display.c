#include "led_display.h"
#include "global.h"
#include "error.h"
#include "time_config.h"
#include <stdio.h>

/**
  * @brief  Initialize display
  */
void display_init(void) {
    lcd_init();
    draw_initial_screen();
}

/**
  * @brief  Draw initial screen
  */
void draw_initial_screen(void) {
    lcd_Clear(BLACK);
    lcd_Fill(0, 0, 240, 30, BLUE);
    lcd_StrCenter(0, 5, "TRAFFIC LIGHT SYSTEM", WHITE, BLUE, 16, 1);
}

/**
  * @brief  Clear specific display area
  */
void clear_display_area(uint16_t x1, uint16_t y1, uint16_t x2, uint16_t y2) {
    lcd_Fill(x1, y1, x2, y2, BLACK);
}

/**
  * @brief  Draw traffic lights on LCD
  */
void draw_traffic_lights(void) {
    uint16_t red1_color = BLACK, yellow1_color = BLACK, green1_color = BLACK;
    uint16_t red2_color = BLACK, yellow2_color = BLACK, green2_color = BLACK;
    
    if (current_mode == MODE_NORMAL) {
        // Determine which lights should be on
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
                break;
        }
    } else {
        // Modification mode: blink corresponding color
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
void draw_mode_info(void) {
    char info_str[30];
    
    // Clear info area
    clear_display_area(0, 250, 240, 320);
    
    if (current_mode == MODE_NORMAL) {
        sprintf(info_str, "MODE: NORMAL");
        lcd_ShowStr(10, 255, info_str, CYAN, BLACK, 16, 0);
        
        // Show countdown
        uint8_t duration = get_current_state_duration();
        uint16_t elapsed = TICKS_TO_SEC(state_counter);
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

/**
  * @brief  Display error message
  */
void display_error(ErrorCode error) {
    if (error == ERROR_NONE) return;
    
    char err_str[30];
    sprintf(err_str, "ERROR: %s", get_error_string(error));
    lcd_ShowStr(10, 35, err_str, RED, BLACK, 16, 0);
}

/**
  * @brief  Update entire display
  */
void update_display(void) {
    draw_traffic_lights();
    draw_mode_info();
    
    if (get_error() != ERROR_NONE) {
        display_error(get_error());
    }
}
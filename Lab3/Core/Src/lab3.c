#include "lab3.h"

// ============================================================================
// Global Variables
// ============================================================================
uint8_t state = BEGIN;
uint8_t clock_count = 0;
int8_t clock[4] = {0, 0, 0, 0};
bool past_1_second = 0;

int8_t time_light_1 = 0;
int8_t time_light_2 = 0;

int8_t config_red_time = INIT_RED_TIME;
int8_t config_green_time = INIT_GREEN_TIME;
int8_t config_yellow_time = INIT_YELLOW_TIME;

int8_t temp_mod_time = 0;
uint8_t blink_counter = 0;
bool manual = 0;

// ============================================================================
// Static Helper Functions
// ============================================================================
static void write_lcd(char *str, uint16_t textColor, uint16_t bgColor) {
    lcd_Fill(0, 30, 240, 54, WHITE);
    lcd_StrCenter(0, 30, str, textColor, bgColor, 24, 0);
}

static void clear_all_lights(void) {
    // Light 1
    lcd_DrawCircle(60, 120, BLACK, 20, 1);
    lcd_DrawCircle(120, 120, BLACK, 20, 1);
    lcd_DrawCircle(180, 120, BLACK, 20, 1);
    
    // Light 2
    lcd_DrawCircle(60, 220, BLACK, 20, 1);
    lcd_DrawCircle(120, 220, BLACK, 20, 1);
    lcd_DrawCircle(180, 220, BLACK, 20, 1);
}

static void set_light_1(uint16_t red, uint16_t green, uint16_t yellow) {
    lcd_DrawCircle(60, 120, red, 20, 1);
    lcd_DrawCircle(120, 120, green, 20, 1);
    lcd_DrawCircle(180, 120, yellow, 20, 1);
}

static void set_light_2(uint16_t red, uint16_t green, uint16_t yellow) {
    lcd_DrawCircle(60, 220, red, 20, 1);
    lcd_DrawCircle(120, 220, green, 20, 1);
    lcd_DrawCircle(180, 220, yellow, 20, 1);
}

static void handle_mode_button_to_red(void) {
    if (button_count[BTN_MODE] == 1) {
        manual = 1;
        temp_mod_time = config_red_time + 1;
        state = MOD_RED;
        blink_counter = 0;
        write_lcd("MODIFICATION", WHITE, RED);
    }
}

static void handle_modification_buttons(int8_t *config_time) {
    // BTN_INC: Increment temporary modification time
    if (button_count[BTN_INC] == 1) {
        temp_mod_time++;
        if (temp_mod_time > MAX_TIME) 
            temp_mod_time = MIN_TIME;
    }
    
    // BTN_SET: Save the configuration
    if (button_count[BTN_SET] == 1) {
        *config_time = temp_mod_time - 1;
    }
}

// ============================================================================
// Public Functions
// ============================================================================
void write_traffic_light_1(void) {
    lcd_ShowStr(10, 60, "Light 1", BLACK, WHITE, 24, 0);
    lcd_ShowStr(175, 60, "00", RED, BLACK, 24, 0);
    lcd_DrawCircle(60, 120, BLACK, 20, 1);
    lcd_DrawCircle(120, 120, BLACK, 20, 1);
    lcd_DrawCircle(180, 120, BLACK, 20, 1);
}

void write_traffic_light_2(void) {
    lcd_ShowStr(10, 160, "Light 2", BLACK, WHITE, 24, 0);
    lcd_ShowStr(175, 160, "00", RED, BLACK, 24, 0);
    lcd_DrawCircle(60, 220, BLACK, 20, 1);
    lcd_DrawCircle(120, 220, BLACK, 20, 1);
    lcd_DrawCircle(180, 220, BLACK, 20, 1);
}

bool validate_traffic_time(void) {
    // Validate: Red = Green + Yellow + 2
    if (config_red_time + 1 != config_green_time + config_yellow_time + 2) {
        // Reset to default values on error
        config_green_time = INIT_GREEN_TIME;
        config_red_time = INIT_RED_TIME;
        config_yellow_time = INIT_YELLOW_TIME;
        
        lcd_Fill(80, 260, 160, 290, WHITE);
        lcd_ShowStr(90, 260, "Error", RED, BLACK, 24, 0);
        return 0;
    }
    return 1;
}

void fsm_traffic(void) {
    switch (state) {
        // ====================================================================
        // BEGIN State
        // ====================================================================
        case BEGIN:
            if (!validate_traffic_time()) {
                lcd_Fill(80, 260, 160, 284, RED);
                lcd_ShowStr(90, 260, "Error", RED, BLACK, 24, 0);
            }
            write_lcd("NORMAL", WHITE, BLACK);
            
            state = RED_GREEN;
            time_light_1 = config_red_time;
            time_light_2 = config_green_time;
            break;

        // ====================================================================
        // RED_GREEN State
        // ====================================================================
        case RED_GREEN:
            handle_mode_button_to_red();
            HAL_GPIO_WritePin(OUTPUT_Y0_GPIO_Port, OUTPUT_Y0_Pin, LEDON);
            
            set_light_1(RED, BLACK, BLACK);
            set_light_2(BLACK, GREEN, BLACK);
            
            if (past_1_second) {
                if (time_light_2 < 0) {
                    state = RED_YELLOW;
                    time_light_2 = config_yellow_time;
                }
                if (time_light_2 == config_green_time - 1) {
                    lcd_Fill(80, 260, 160, 290, WHITE);
                }
            }
            break;

        // ====================================================================
        // RED_YELLOW State
        // ====================================================================
        case RED_YELLOW:
            handle_mode_button_to_red();
            
            set_light_1(RED, BLACK, BLACK);
            set_light_2(BLACK, BLACK, YELLOW);
            
            if (past_1_second) {
                if (time_light_1 < 0) {
                    state = GREEN_RED;
                    clock[1] = 3;
                    time_light_1 = config_green_time;
                    time_light_2 = config_red_time;
                }
            }
            break;

        // ====================================================================
        // GREEN_RED State
        // ====================================================================
        case GREEN_RED:
            handle_mode_button_to_red();
            
            set_light_1(BLACK, GREEN, BLACK);
            set_light_2(RED, BLACK, BLACK);
            
            if (past_1_second) {
                if (time_light_1 < 0) {
                    state = YELLOW_RED;
                    clock[1] = 2;
                    time_light_1 = config_yellow_time;
                }
            }
            break;

        // ====================================================================
        // YELLOW_RED State
        // ====================================================================
        case YELLOW_RED:
            handle_mode_button_to_red();
            
            set_light_1(BLACK, BLACK, YELLOW);
            set_light_2(RED, BLACK, BLACK);
            
            if (past_1_second) {
                if (time_light_2 < 0) {
                    state = RED_GREEN;
                    clock[1] = 5;
                    time_light_1 = config_red_time;
                    time_light_2 = config_green_time;
                }
            }
            break;

        // ====================================================================
        // MOD_RED State
        // ====================================================================
        case MOD_RED:
            HAL_GPIO_WritePin(DEBUG_LED_GPIO_Port, DEBUG_LED_Pin, LEDON);
            
            // Turn off green and yellow lights
            set_light_1(BLACK, BLACK, BLACK);
            set_light_2(BLACK, BLACK, BLACK);
            
            // Blink red lights
            if (blink_counter < 10) {
                lcd_DrawCircle(60, 120, RED, 20, 1);
                lcd_DrawCircle(60, 220, RED, 20, 1);
            } else {
                lcd_DrawCircle(60, 120, BLACK, 20, 1);
                lcd_DrawCircle(60, 220, BLACK, 20, 1);
            }
            
            handle_modification_buttons(&config_red_time);
            
            // BTN_MODE: Move to green modification
            if (button_count[BTN_MODE] == 1) {
                state = MOD_GREEN;
                temp_mod_time = config_green_time + 1;
                blink_counter = 0;
                write_lcd("MODIFICATION", WHITE, GREEN);
            }
            break;

        // ====================================================================
        // MOD_GREEN State
        // ====================================================================
        case MOD_GREEN:
            // Turn off red and yellow lights
            set_light_1(BLACK, BLACK, BLACK);
            set_light_2(BLACK, BLACK, BLACK);
            
            // Blink green lights
            if (blink_counter < 10) {
                lcd_DrawCircle(120, 120, GREEN, 20, 1);
                lcd_DrawCircle(120, 220, GREEN, 20, 1);
            } else {
                lcd_DrawCircle(120, 120, BLACK, 20, 1);
                lcd_DrawCircle(120, 220, BLACK, 20, 1);
            }
            
            handle_modification_buttons(&config_green_time);
            
            // BTN_MODE: Move to yellow modification
            if (button_count[BTN_MODE] == 1) {
                state = MOD_YELLOW;
                temp_mod_time = config_yellow_time + 1;
                blink_counter = 0;
                write_lcd("MODIFICATION", WHITE, YELLOW);
            }
            break;

        // ====================================================================
        // MOD_YELLOW State
        // ====================================================================
        case MOD_YELLOW:
            // Turn off red and green lights
            set_light_1(BLACK, BLACK, BLACK);
            set_light_2(BLACK, BLACK, BLACK);
            
            // Blink yellow lights
            if (blink_counter < 10) {
                lcd_DrawCircle(180, 120, YELLOW, 20, 1);
                lcd_DrawCircle(180, 220, YELLOW, 20, 1);
            } else {
                lcd_DrawCircle(180, 120, BLACK, 20, 1);
                lcd_DrawCircle(180, 220, BLACK, 20, 1);
            }
            
            handle_modification_buttons(&config_yellow_time);
            
            // BTN_MODE: Exit modification mode
            if (button_count[BTN_MODE] == 1) {
                manual = 0;
                state = BEGIN;
                HAL_GPIO_WritePin(DEBUG_LED_GPIO_Port, DEBUG_LED_Pin, LEDOFF);
                blink_counter = 0;
            }
            break;

        default:
            break;
    }
}

void led7_display(int8_t light1, int8_t light2) {
    char str_light_1[5];
    char str_light_2[5];
    
    snprintf(str_light_1, 5, "%02d", light1);
    snprintf(str_light_2, 5, "%02d", light2);
    
    lcd_ShowStr(175, 60, str_light_1, RED, BLACK, 24, 0);
    lcd_ShowStr(175, 160, str_light_2, RED, BLACK, 24, 0);
}
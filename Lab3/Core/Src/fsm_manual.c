#include "fsm_manual.h"
#include "button.h"
#include "time_config.h"
#include "error.h"

/**
  * @brief  Initialize manual FSM
  */
void fsm_manual_init(void) {
    temp_duration = 0;
}

/**
  * @brief  Validate duration value
  */
int validate_duration(uint8_t duration) {
    return (duration >= MIN_DURATION && duration <= MAX_DURATION);
}

/**
  * @brief  Handle mode button (Button 0)
  */
void handle_button_mode(void) {
    if (button_count[BTN_MODE] == 1) {  // Short press
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
void handle_button_increase(void) {
    if (current_mode == MODE_NORMAL) return;
    
    // Short press: increase by 1
    if (button_count[BTN_INC] == 1) {
        temp_duration++;
        if (temp_duration > MAX_DURATION) temp_duration = MIN_DURATION;
    }
    // Long press: fast increment
    else if (button_count[BTN_INC] > LONG_PRESS_THRESHOLD && 
             button_count[BTN_INC] % FAST_INCREMENT_INTERVAL == 0) {
        temp_duration++;
        if (temp_duration > MAX_DURATION) temp_duration = MIN_DURATION;
    }
    
    // Validate
    if (!validate_duration(temp_duration)) {
        temp_duration = MIN_DURATION;
        set_error(ERROR_INVALID_DURATION);
    }
}

/**
  * @brief  Handle confirm button (Button 2)
  */
void handle_button_confirm(void) {
    if (current_mode == MODE_NORMAL) return;
    
    if (button_count[BTN_CONFIRM] == 1) {  // Short press
        // Validate before saving
        if (!validate_duration(temp_duration)) {
            set_error(ERROR_INVALID_DURATION);
            return;
        }
        
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
  * @brief  Run manual mode FSM
  */
void fsm_manual_run(void) {
    handle_button_mode();
    handle_button_increase();
    handle_button_confirm();
}
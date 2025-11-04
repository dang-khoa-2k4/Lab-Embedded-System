#include "error.h"
#include "led_display.h"
#include "global.h"

/* Global Error Variable */
ErrorCode current_error = ERROR_NONE;

/**
  * @brief  Set error code
  */
void set_error(ErrorCode error) {
    current_error = error;
}

/**
  * @brief  Clear error
  */
void clear_error(void) {
    current_error = ERROR_NONE;
}

/**
  * @brief  Get current error
  */
ErrorCode get_error(void) {
    return current_error;
}

/**
  * @brief  Get error description string
  */
const char* get_error_string(ErrorCode error) {
    switch (error) {
        case ERROR_NONE:            return "No Error";
        case ERROR_INVALID_STATE:   return "Invalid State";
        case ERROR_INVALID_MODE:    return "Invalid Mode";
        case ERROR_INVALID_DURATION:return "Invalid Duration";
        case ERROR_DISPLAY_FAIL:    return "Display Failure";
        case ERROR_BUTTON_FAIL:     return "Button Failure";
        case ERROR_TIMER_FAIL:      return "Timer Failure";
        case ERROR_INIT_FAIL:       return "Init Failure";
        default:                    return "Unknown Error";
    }
}

/**
  * @brief  Handle error (display and recover)
  */
void handle_error(ErrorCode error) {
    set_error(error);
    display_error(error);
    
    // Recovery logic
    if (error == ERROR_INVALID_STATE) {
        fsm_reset();
    }
}

/**
  * @brief  Reset system to safe state
  */
void system_reset(void) {
    // Reset to default values
    red_duration = DEFAULT_RED_DURATION;
    green_duration = DEFAULT_GREEN_DURATION;
    yellow_duration = DEFAULT_YELLOW_DURATION;
    
    current_mode = MODE_NORMAL;
    traffic_state = STATE_RED1_RED2;  // Safe state
    state_counter = 0;
    
    clear_error();
}

/**
  * @brief  Reset FSM to initial state
  */
void fsm_reset(void) {
    traffic_state = STATE_GREEN1_RED2;
    state_counter = 0;
    clear_error();
}
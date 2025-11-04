#include "fsm_automatic.h"
#include "time_config.h"
#include "error.h"

/**
  * @brief  Check if traffic state is valid
  */
int is_valid_traffic_state(TrafficState state) {
    return (state >= STATE_GREEN1_RED2 && state <= STATE_RED1_RED2);
}

/**
  * @brief  Initialize automatic FSM
  */
void fsm_automatic_init(void) {
    traffic_state = STATE_GREEN1_RED2;
    state_counter = 0;
}

/**
  * @brief  Get duration for current traffic state
  */
uint8_t get_current_state_duration(void) {
    switch (traffic_state) {
        case STATE_GREEN1_RED2:
        case STATE_RED1_GREEN2:
            return green_duration;
        case STATE_YELLOW1_RED2:
        case STATE_RED1_YELLOW2:
            return yellow_duration;
        case STATE_RED1_RED2:
            return 1;  // Emergency state
        default:
            set_error(ERROR_INVALID_STATE);
            return 1;
    }
}

/**
  * @brief  Validate all durations
  */
void validate_durations(void) {
    if (red_duration < MIN_DURATION) red_duration = MIN_DURATION;
    if (red_duration > MAX_DURATION) red_duration = MAX_DURATION;
    
    if (green_duration < MIN_DURATION) green_duration = MIN_DURATION;
    if (green_duration > MAX_DURATION) green_duration = MAX_DURATION;
    
    if (yellow_duration < MIN_DURATION) yellow_duration = MIN_DURATION;
    if (yellow_duration > MAX_DURATION) yellow_duration = MAX_DURATION;
}

/**
  * @brief  Get next state in cycle
  */
TrafficState get_next_state(TrafficState current) {
    switch (current) {
        case STATE_GREEN1_RED2:
            return STATE_YELLOW1_RED2;
        case STATE_YELLOW1_RED2:
            return STATE_RED1_GREEN2;
        case STATE_RED1_GREEN2:
            return STATE_RED1_YELLOW2;
        case STATE_RED1_YELLOW2:
            return STATE_GREEN1_RED2;
        default:
            set_error(ERROR_INVALID_STATE);
            return STATE_RED1_RED2;  // Safe state
    }
}

/**
  * @brief  Update traffic FSM (called every 50ms)
  */
void fsm_automatic_run(void) {
    state_counter++;
    
    // Calculate duration in ticks
    uint16_t duration_in_ticks = SEC_TO_TICKS(get_current_state_duration());
    
    if (state_counter >= duration_in_ticks) {
        state_counter = 0;
        
        // Transition to next state
        traffic_state = get_next_state(traffic_state);
        
        // Validate durations after state change
        validate_durations();
    }
}
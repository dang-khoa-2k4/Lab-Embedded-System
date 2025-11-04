#ifndef INC_ERROR_H_
#define INC_ERROR_H_

#include <stdint.h>

/* Error Codes */
typedef enum {
    ERROR_NONE = 0,
    ERROR_INVALID_STATE,
    ERROR_INVALID_MODE,
    ERROR_INVALID_DURATION,
    ERROR_DISPLAY_FAIL,
    ERROR_BUTTON_FAIL,
    ERROR_TIMER_FAIL,
    ERROR_INIT_FAIL
} ErrorCode;

/* Global Error Variable */
extern ErrorCode current_error;

/* Error Handling Functions */
void set_error(ErrorCode error);
void clear_error(void);
ErrorCode get_error(void);
const char* get_error_string(ErrorCode error);
void handle_error(ErrorCode error);

/* System Recovery Functions */
void system_reset(void);
void fsm_reset(void);

#endif /* INC_ERROR_H_ */
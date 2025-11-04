/*
 * clock.h
 *
 *  Created on: Nov 3, 2023
 *      Author: HaHuyen
 */

#ifndef INC_CLOCK_H_
#define INC_CLOCK_H_

#include "main.h"
#include <stdint.h>

// Mode Definitions
typedef enum {
    MODE_VIEW = 0,   // Normal display mode
    MODE_EDIT,       // Time/date edit mode
    MODE_ALARM,      // Alarm setting mode
    MODE_UPDATE      // UART update mode
} ClockMode;

// Parameter Definitions
typedef enum {
    PARAM_HOUR = 0,  // Hours (0-23)
    PARAM_MIN,       // Minutes (0-59)
    PARAM_SEC,       // Seconds (0-59)
    PARAM_DAY,       // Day (1-7)
    PARAM_DATE,      // Date (1-31)
    PARAM_MONTH,     // Month (1-12)
    PARAM_YEAR,      // Year (0-99)
    PARAM_COUNT      // Total number of parameters
} EditParam;

// Constants
#define RING_BUFFER_SIZE   128    // UART ring buffer size
#define TIMEOUT_MS         10000  // UART timeout (10 seconds)
#define MAX_RETRY          3      // Maximum retry attempts
#define LONG_PRESS_TIME    40     // Long press detection time
#define FAST_INCREMENT     4      // Fast increment interval

// Function Prototypes
void clock_init(void);
void clock_handle_buttons(void);
void clock_display(void);
void clock_update_mode_handler(void);
void clock_process_uart(void);

#endif /* INC_CLOCK_H_ */
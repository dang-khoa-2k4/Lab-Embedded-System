#ifndef INC_TIME_CONFIG_H_
#define INC_TIME_CONFIG_H_

/* Time Constants */
#define TICKS_PER_SECOND        20      // 50ms per tick = 20 ticks/second
#define MS_PER_TICK             50      // Milliseconds per tick

/* Duration Limits */
#define MAX_DURATION            99
#define MIN_DURATION            1

/* Default Durations (seconds) */
#define DEFAULT_RED_DURATION    10
#define DEFAULT_GREEN_DURATION  8
#define DEFAULT_YELLOW_DURATION 2

/* Button Timing */
#define BUTTON_SCAN_INTERVAL    50      // 50ms
#define LONG_PRESS_THRESHOLD    40      // 2 seconds (40 * 50ms)
#define FAST_INCREMENT_INTERVAL 4       // Speed up after long press (every 4 ticks)

/* LED Blink Timing */
#define BLINK_INTERVAL          500     // 500ms (2Hz)

/* Time Conversion Macros */
#define SEC_TO_TICKS(x)         ((x) * TICKS_PER_SECOND)
#define TICKS_TO_SEC(x)         ((x) / TICKS_PER_SECOND)
#define MS_TO_TICKS(x)          ((x) / MS_PER_TICK)

/* Timer IDs */
#define TIMER_TRAFFIC           1       // Traffic light cycle timer
#define TIMER_BUTTON_SCAN       2       // Button scan timer
#define TIMER_BLINK             3       // LED blink timer

#endif /* INC_TIME_CONFIG_H_ */
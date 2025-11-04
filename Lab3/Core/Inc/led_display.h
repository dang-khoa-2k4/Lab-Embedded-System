#ifndef INC_LED_DISPLAY_H_
#define INC_LED_DISPLAY_H_

#include "global.h"
#include "error.h"
#include "lcd.h"
#include <stdint.h>

/* Display Initialization */
void display_init(void);

/* LCD Display Functions */
void draw_traffic_lights(void);
void draw_mode_info(void);
void draw_initial_screen(void);
void clear_display_area(uint16_t x1, uint16_t y1, uint16_t x2, uint16_t y2);

/* 7-Segment Display Functions (if needed) */
void display7SEG(int num);
void update7SEG(int index);

/* Error Display */
void display_error(ErrorCode error);

/* Display Update */
void update_display(void);

#endif /* INC_LED_DISPLAY_H_ */
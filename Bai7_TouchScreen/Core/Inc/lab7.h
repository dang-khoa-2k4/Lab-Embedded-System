#include "main.h"

typedef enum{
    INIT,
    DRAW,
    START,
    CLEAR
} DrawStatus;

// Chỉ KHAI BÁO
extern DrawStatus draw_Status;
extern uint8_t button[4];

void touchProcess();
uint8_t isButtonClear();
uint8_t isButtonStart();
uint8_t isButtonUp();
uint8_t isButtonDown();
uint8_t isButtonLeft();
uint8_t isButtonRight();

void lcd_DrawTriangle(uint16_t x1, uint16_t y1, uint16_t x2, uint16_t y2, uint16_t x3, uint16_t y3, uint16_t color);
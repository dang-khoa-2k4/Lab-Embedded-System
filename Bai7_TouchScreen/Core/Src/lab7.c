#include "lab7.h"

DrawStatus draw_Status = INIT;
uint8_t button[4] = {0}; // Up : 0 ; Down: 1; Left: 2; Right: 3

uint8_t isButtonClear()
{
    if (!touch_IsTouched())
        return 0;
    return touch_GetX() > 60 && touch_GetX() < 180 && touch_GetY() > 80 && touch_GetY() < 130;
}
uint8_t isButtonStart()
{
    if (!touch_IsTouched())
        return 0;
    return touch_GetX() > 60 && touch_GetX() < 180 && touch_GetY() > 10 && touch_GetY() < 60;
}
uint8_t isButtonUp()
{
    if (!touch_IsTouched())
        return 0;
    return touch_GetX() > 100 && touch_GetX() < 140 && touch_GetY() > 200 && touch_GetY() < 250;
}
uint8_t isButtonDown()
{
    if (!touch_IsTouched())
        return 0;
    return touch_GetX() > 100 && touch_GetX() < 140 && touch_GetY() > 270 && touch_GetY() < 320;
}
uint8_t isButtonLeft()
{
    if (!touch_IsTouched())
        return 0;
    return touch_GetX() > 50 && touch_GetX() < 100 && touch_GetY() > 240 && touch_GetY() < 280;
}
uint8_t isButtonRight()
{
    if (!touch_IsTouched())
        return 0;
    return touch_GetX() > 140 && touch_GetX() < 190 && touch_GetY() > 240 && touch_GetY() < 280;
}

// Thêm hàm vẽ tam giác đầy
void lcd_DrawTriangle(uint16_t x1, uint16_t y1, uint16_t x2, uint16_t y2, uint16_t x3, uint16_t y3, uint16_t color)
{
    // Vẽ 3 cạnh của tam giác
    lcd_DrawLine(x1, y1, x2, y2, color);
    lcd_DrawLine(x2, y2, x3, y3, color);
    lcd_DrawLine(x3, y3, x1, y1, color);

    // Tô đầy tam giác (scan line algorithm đơn giản)
    int16_t minY = y1 < y2 ? (y1 < y3 ? y1 : y3) : (y2 < y3 ? y2 : y3);
    int16_t maxY = y1 > y2 ? (y1 > y3 ? y1 : y3) : (y2 > y3 ? y2 : y3);

    for (int16_t y = minY; y <= maxY; y++)
    {
        int16_t minX = 240, maxX = 0;

        // Tìm điểm giao của scan line với các cạnh
        if ((y >= y1 && y <= y2) || (y >= y2 && y <= y1))
        {
            int16_t x = x1 + (x2 - x1) * (y - y1) / (y2 - y1 + 1);
            if (x < minX)
                minX = x;
            if (x > maxX)
                maxX = x;
        }
        if ((y >= y2 && y <= y3) || (y >= y3 && y <= y2))
        {
            int16_t x = x2 + (x3 - x2) * (y - y2) / (y3 - y2 + 1);
            if (x < minX)
                minX = x;
            if (x > maxX)
                maxX = x;
        }
        if ((y >= y3 && y <= y1) || (y >= y1 && y <= y3))
        {
            int16_t x = x3 + (x1 - x3) * (y - y3) / (y1 - y3 + 1);
            if (x < minX)
                minX = x;
            if (x > maxX)
                maxX = x;
        }

        // Vẽ đường ngang
        if (minX <= maxX)
        {
            lcd_DrawLine(minX, y, maxX, y, color);
        }
    }
}

void touchProcess()
{
    switch (draw_Status)
    {
    case INIT:
        // display blue button
        // lcd_Fill(60, 10, 180, 60, GBLUE);
        // lcd_ShowStr(90, 20, "START", RED, BLACK, 24, 1);

        // lcd_Fill(60, 80, 180, 130, GBLUE);
        // lcd_ShowStr(90, 90, "RESET", RED, BLACK, 24, 1);
        // UP - Tam giác hướng lên
        lcd_DrawTriangle(
            120, 200, // Đỉnh trên (giữa)
            100, 250, // Đỉnh trái dưới
            140, 250, // Đỉnh phải dưới
            RED);

        // DOWN - Tam giác hướng xuống
        lcd_DrawTriangle(
            120, 320, // Đỉnh dưới (giữa)
            100, 270, // Đỉnh trái trên
            140, 270, // Đỉnh phải trên
            RED);

        // LEFT - Tam giác hướng trái
        lcd_DrawTriangle(
            50, 260,  // Đỉnh trái (giữa)
            100, 240, // Đỉnh phải trên
            100, 280, // Đỉnh phải dưới
            RED);

        // RIGHT - Tam giác hướng phải
        lcd_DrawTriangle(
            190, 260, // Đỉnh phải (giữa)
            140, 240, // Đỉnh trái trên
            140, 280, // Đỉnh trái dưới
            RED);

        draw_Status = DRAW;
        break;
    case DRAW:
        if (isButtonClear())
        {
            draw_Status = CLEAR;
            // clear board
            lcd_Fill(0, 0, 240, 320, BLACK);
            // display green button
            lcd_Fill(60, 10, 180, 60, GREEN);
            lcd_ShowStr(90, 20, "RESET", RED, BLACK, 24, 1);
        }
        if (isButtonStart())
        {
            draw_Status = START;
            lcd_Fill(0, 0, 240, 320, BLACK);

            game_Init();
        }

        break;
    case START:
        // 240X320
        lcd_Fill(110, 230, 130, 250, RED);
        lcd_Fill(110, 270, 130, 290, RED);

        lcd_Fill(80, 250, 100, 270, RED);
        lcd_Fill(140, 250, 160, 270, RED);

        if (is_game_over)
        {
            lcd_ShowStr(80, 100, "GAME OVER", WHITE, BLACK, 24, 1);
            if (isButtonClear() || isButtonStart())
            {
                draw_Status = CLEAR;
            }
            break;
        }
        if (isButtonUp() && direction != 1)
        {
            direction = 0;
        }
        else if (isButtonDown() && direction != 0)
        {
            direction = 1;
        }
        else if (isButtonLeft() && direction != 3)
        {
            direction = 2;
        }
        else if (isButtonRight() && direction != 2)
        {
            direction = 3;
        }
        game_tick_counter++;
        if (game_tick_counter >= SNAKE_SPEED)
        {
            game_tick_counter = 0;
            update_Game();
        }
        break;
    case CLEAR:
        if (!touch_IsTouched())
            draw_Status = INIT;
        break;
    default:
        break;
    }
}
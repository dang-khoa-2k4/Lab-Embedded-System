
#include "main.h"

#ifndef INC_SNAKE_H_
#define INC_SNAKE_H_

#define MAX_SNAKE_LEN 100
#define GRID_SIZE 10
#define GRID_WIDTH 24
#define GRID_HEIGHT 22

#define SNAKE_SPEED 4 	// 4 * 50ms

#define NUM_COLORS 10

typedef struct {
    int x;
    int y;
    uint32_t color;
} Point;


extern Point snake[MAX_SNAKE_LEN];
extern char score_str[20];
extern int snake_length;
extern Point food;
extern int direction; // 0: Up, 1: Down, 2: Left, 3: Right
extern int score;
extern int is_game_over;
extern int game_tick_counter;


void draw_GridSquare(int x, int y, uint16_t color);

void spawn_Food(void);


void game_Init(void);


void update_Game(void);

#endif /* INC_SNAKE_H_ */

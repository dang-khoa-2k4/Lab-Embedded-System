
#include "snake.h"

Point snake[MAX_SNAKE_LEN];
char score_str[20];
int snake_length = 3;
Point food;
int direction = 0; // 0: up, 1: down, 2: left, 3: right
int score = 0;
int is_game_over = 0;
int game_tick_counter = 0;

void draw_GridSquare(int x, int y, uint16_t color) {
    lcd_Fill(x * GRID_SIZE, y * GRID_SIZE,
             (x + 1) * GRID_SIZE - 1, (y + 1) * GRID_SIZE - 1,
             color);
}


void spawn_Food() {
    while(1) {
        food.x = rand() % GRID_WIDTH;
        food.y = rand() % GRID_HEIGHT;

        int on_snake = 0;
        for (int i = 0; i < snake_length; i++) {
            if (snake[i].x == food.x && snake[i].y == food.y) {
                on_snake = 1;
                break;
            }
        }
        if (!on_snake) break;
    }
    draw_GridSquare(food.x, food.y, GREEN);
}


void game_Init() {
    lcd_Fill(0, 0, 240, 220, BLACK);

    snake_length = 3;
    snake[0].x = GRID_WIDTH / 2;
    snake[0].y = GRID_HEIGHT / 2;
    snake[1].x = GRID_WIDTH / 2 - 1;
    snake[1].y = GRID_HEIGHT / 2;
    snake[2].x = GRID_WIDTH / 2 - 2;
    snake[2].y = GRID_HEIGHT / 2;

    // draw map boundaries
    for (int x = 0; x < GRID_WIDTH; x++) {
        draw_GridSquare(x, 0, BLUE);
        draw_GridSquare(x, GRID_HEIGHT - 1, BLUE);
    }
    for (int y = 0; y < GRID_HEIGHT; y++) {
        draw_GridSquare(0, y, BLUE);
        draw_GridSquare(GRID_WIDTH - 1, y, BLUE);
    }

    direction = 3;
    score = 0;
    is_game_over = 0;
    game_tick_counter = 0;

    for (int i = 0; i < snake_length; i++) {
        draw_GridSquare(snake[i].x, snake[i].y, WHITE);
    }

    spawn_Food();
}


void update_Game() {
    if (is_game_over) return;

    Point new_head = snake[0];
    if (direction == 0) new_head.y--;
    if (direction == 1) new_head.y++;
    if (direction == 2) new_head.x--;
    if (direction == 3) new_head.x++;

    if (new_head.x < 0 || new_head.x >= GRID_WIDTH ||
        new_head.y < 0 || new_head.y >= GRID_HEIGHT) {
        is_game_over = 1;
        return;
    }

    for (int i = 1; i < snake_length; i++) {
        if (snake[i].x == new_head.x && snake[i].y == new_head.y) {
            is_game_over = 1;
            return;
        }
    }


    int ate_food = 0;
    if (new_head.x == food.x && new_head.y == food.y) {
        ate_food = 1;
        score++;
        snake_length++;
        if (snake_length > MAX_SNAKE_LEN) snake_length = MAX_SNAKE_LEN;
        // update display score
        char score_str[20];
        sprintf(score_str, "Score: %d", score);
        lcd_Fill(0, 220, 240, 240, BLACK);
        lcd_ShowStr(0, 220, score_str, WHITE, BLACK, 16, 0);
    }

    if (!ate_food) {
        draw_GridSquare(snake[snake_length - 1].x, snake[snake_length - 1].y, BLACK);
    }

    for (int i = snake_length - 1; i > 0; i--) {
        snake[i] = snake[i - 1];
    }

    snake[0] = new_head;
    draw_GridSquare(snake[0].x, snake[0].y, WHITE);

    if (ate_food) {
        spawn_Food();
    }
}

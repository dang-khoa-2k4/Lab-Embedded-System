#include "main.h"

#define LEDON			1
#define LEDOFF			0
#define BEGIN			10
#define RED_GREEN		100
#define RED_YELLOW		101
#define GREEN_RED		102
#define YELLOW_RED		103

#define MOD_RED			12
#define MOD_GREEN		13
#define MOD_YELLOW		14

#define BTN_MODE		5
#define BTN_INC			6
#define BTN_SET			7


#define INIT_RED_TIME		5 - 1
#define INIT_GREEN_TIME		3 - 1
#define INIT_YELLOW_TIME	2 - 1

#define MAX_TIME		99
#define MIN_TIME		1


extern uint8_t state;
extern uint8_t clock_count;
extern int8_t clock[4];
extern bool past_1_second;

extern int8_t time_light_1;
extern int8_t time_light_2;

extern int8_t config_red_time;
extern int8_t config_green_time;
extern int8_t config_yellow_time;

extern int8_t temp_mod_time;
extern uint8_t blink_counter;
extern bool manual;

void fsm_traffic(void);
void write_traffic_light_1(void);
void write_traffic_light_2(void);
void led7_display(int8_t light1, int8_t light2);
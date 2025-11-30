//#ifndef INC_SCHEDULER_H_
//#define INC_SCHEDULER_H_
//#include "stdlib.h"
//#include "main.h"
//
//#define TIMER_CYCLE 1
//
//typedef struct sTask {
//// Pointer to the task (must be a ’ void ( void ) ’ function )
//	void ( * pTask) ( void ) ;
//	// Delay ( ticks ) until the function will ( next ) be run
//	int Delay ;
//	// Interval ( tickse ) between subsequent runs .
//	uint32_t Period ;
//	// Incremented ( by scheduler ) when task is due to execute
//	uint8_t RunMe;
//	// This is a hint to solve the question below .
//	uint32_t TaskID ;
//	struct sTask * next;
//} sTask;
//
//extern sTask * SCH_Tasks;
//
//extern unsigned int taskID;
//
//void SCH_Init ( void );
//unsigned int SCH_Add_Task ( void (* Task) ( void ), uint32_t Delay, uint32_t Period );
//void SCH_Add_Task_Dispatcher ( void (* Task) ( void ), uint32_t Delay, uint32_t Period, int TaskID );
//
//void Delete_Task_Dispatcher ( void );
//void Delete_Task ( unsigned int TaskID );
//
//void SCH_Update ( void );
//void SCH_Dispatcher ( void );
////void SCH_Display();
//
//#endif /* INC_SCHEDULER_H_ */

#ifndef INC_SCHEDULER_H_
#define INC_SCHEDULER_H_

#include "main.h"
#include "stdint.h"
#include "RingBuffer.h"

#define MAX_TASKS 10
#define TIMER_CYCLE 1

typedef struct {
	uint16_t taskID;
    void (*pTask)(void);
    uint32_t Delay;
    uint32_t Period;
    uint8_t isUsed;
} sTask;

void SCH_Init(void);
int SCH_Add_Task(void (*pFunction)(), uint32_t Delay, uint32_t Period);
int SCH_ReAdd_Task(void (*pFunction)(), uint32_t Delay, uint32_t Period, uint16_t taskID);
int SCH_Delete_Task(uint16_t taskID, uint8_t give_back_taskID);
void SCH_Update(void);
void SCH_Dispatcher(void);

#endif /* INC_SCHEDULER_H_ */

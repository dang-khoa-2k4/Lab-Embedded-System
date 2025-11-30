//#include "Scheduler.h"
//
//
//sTask * SCH_Tasks;
//unsigned int taskID = 0;
//
//void SCH_Init ( void ) {
//	SCH_Tasks = NULL;
//}
//
//unsigned int SCH_Add_Task (void (* Task) (void), uint32_t Delay, uint32_t Period) {
//    sTask * NewTask = (sTask *) malloc (sizeof( sTask ));
//    NewTask->pTask = Task;
//	NewTask->Delay = Delay;
//    NewTask->Period = Period;
//    NewTask->RunMe = 0;
//    NewTask->TaskID = taskID++;
//
//    //Find the suitable position for Task
//    sTask **pointer = &SCH_Tasks;
//    while (*pointer != NULL && NewTask->Delay >= (*pointer)->Delay) {
//        NewTask->Delay -= (*pointer)->Delay;
//        pointer = &((*pointer)->next);
//    }
//    NewTask->next = *pointer;
//    *pointer = NewTask;
//
//    //Update the rest of the linked list
//    if (NewTask->next != NULL) {
//    	(NewTask->next)->Delay -= NewTask->Delay;
//    }
//    return NewTask->TaskID;
//}
//
//void SCH_Add_Task_Dispatcher (void (* Task) (void), uint32_t Delay, uint32_t Period, int TaskID) {
//    sTask * NewTask = (sTask *) malloc (sizeof(sTask));
//    NewTask->pTask = Task;
//    NewTask->Delay = Delay;
//    NewTask->Period = Period;
//    NewTask->RunMe = 0;
//    NewTask->TaskID = TaskID;
//
//    //Find the suitable position for Task
//    sTask **pointer = &SCH_Tasks;
//    while (*pointer != NULL && NewTask->Delay >= (*pointer)->Delay) {
//        NewTask->Delay -= (*pointer)->Delay;
//        pointer = &((*pointer)->next);
//    }
//    NewTask->next = *pointer;
//    *pointer = NewTask;
//
//    //Update the rest of the linked list
//    if (NewTask->next != NULL) {
//    	(NewTask->next)->Delay -= NewTask->Delay;
//    }
//}
//
//void Delete_Task_Dispatcher ( void ) {
//    sTask * deleteTask = SCH_Tasks;
//
//    SCH_Tasks = SCH_Tasks->next;
//    free(deleteTask);
//}
//
//void Delete_Task ( unsigned int TaskID) {
//    sTask ** taskPointer = &SCH_Tasks;
//
//    while (*taskPointer != NULL && (*taskPointer)->TaskID != TaskID)
//        taskPointer = &((*taskPointer)->next);
//
//    if (*taskPointer != NULL) {
//        sTask *deleteTask = *taskPointer;
//        *taskPointer = (*taskPointer)->next;
//        free(deleteTask);
//    }
//}
//
//void SCH_Update ( void ) {
//	if (SCH_Tasks != NULL) {
//		SCH_Tasks->Delay -= TIMER_CYCLE;
//	}
//}
//
//void SCH_Dispatcher ( void ) {
//	if (SCH_Tasks != NULL) {
//		if (SCH_Tasks->Delay <= 0) {
//			(*(SCH_Tasks->pTask))();
//			if (SCH_Tasks->Period > 0) {
//				void *func = SCH_Tasks->pTask;
//				unsigned int Period = SCH_Tasks->Period;
//				unsigned int TaskID = SCH_Tasks->TaskID;
//				SCH_Add_Task_Dispatcher (func, Period, Period, TaskID);
//			}
//			Delete_Task_Dispatcher ();
//		}
//	}
//}
//
////void SCH_Display() {
////    sTask * head = SCH_Tasks;
////    while (head){
////        printf("(Delay: %d, Period: %d, RunMe: %d) ", head->Delay, head->Period, head->RunMe);
////        head = head->next;
////    }
////    printf ("\n");
////}
//
//void HAL_TIM_PeriodElapsedCallback(TIM_HandleTypeDef *htim) {
//	SCH_Update();
//}
#include "Scheduler.h"

sTask SCH_Tasks[MAX_TASKS];

struct RingBuffer taskID_buffer;

void SCH_Init(void) {
    for(int i = 0; i < MAX_TASKS; i++)
        SCH_Tasks[i].isUsed = 0;

    RingBuffer_init(&taskID_buffer, MAX_TASKS);
    for (int idx = 0; idx < MAX_TASKS; idx++)
    {
    	RingBuffer_append(&taskID_buffer, idx);
    }
}

// Insert task while keeping delta timing
int SCH_Add_Task(void (*pFunction)(), uint32_t Delay, uint32_t Period) {
    int i, pos, taskID;

    // Find free slot
    for(pos = 0; pos < MAX_TASKS; pos++)
        if(SCH_Tasks[pos].isUsed == 0) break;
    if(pos == MAX_TASKS) return -1;

    taskID = RingBuffer_pop(&taskID_buffer);
    if (taskID == -1) {
    	return -1;
    }

    // Find sorted insert position
    uint32_t d = Delay;
    for(i = 0; i < MAX_TASKS; i++) {
        if(!SCH_Tasks[i].isUsed || d < SCH_Tasks[i].Delay) break;
        d -= SCH_Tasks[i].Delay;
    }

    // Shift tasks to make space
    for(int j = MAX_TASKS - 1; j > i; j--)
        SCH_Tasks[j] = SCH_Tasks[j-1];

    // Insert new task
    SCH_Tasks[i].taskID = taskID;
    SCH_Tasks[i].pTask = pFunction;
    SCH_Tasks[i].Delay = d;
    SCH_Tasks[i].Period = Period;
    SCH_Tasks[i].isUsed = 1;

    // Fix delta of next task
    if(i + 1 < MAX_TASKS && SCH_Tasks[i+1].isUsed)
        SCH_Tasks[i+1].Delay -= d;

    return taskID;
}
// Insert task while keeping delta timing
int SCH_ReAdd_Task(void (*pFunction)(), uint32_t Delay, uint32_t Period, uint16_t taskID) {
    int i, pos;

    // Find free slot
    for(pos = 0; pos < MAX_TASKS; pos++)
        if(SCH_Tasks[pos].isUsed == 0) break;
    if(pos == MAX_TASKS) return -1;

    if (taskID == -1) {
    	return -1;
    }

    // Find sorted insert position
    uint32_t d = Delay;
    for(i = 0; i < MAX_TASKS; i++) {
        if(!SCH_Tasks[i].isUsed || d < SCH_Tasks[i].Delay) break;
        d -= SCH_Tasks[i].Delay;
    }

    // Shift tasks to make space
    for(int j = MAX_TASKS - 1; j > i; j--)
        SCH_Tasks[j] = SCH_Tasks[j-1];

    // Insert new task
    SCH_Tasks[i].taskID = taskID;
    SCH_Tasks[i].pTask = pFunction;
    SCH_Tasks[i].Delay = d;
    SCH_Tasks[i].Period = Period;
    SCH_Tasks[i].isUsed = 1;

    // Fix delta of next task
    if(i + 1 < MAX_TASKS && SCH_Tasks[i+1].isUsed)
        SCH_Tasks[i+1].Delay -= d;

    return taskID;
}

//void SCH_Delete_Task(uint16_t taskID) {
//	int index;
//
//	for (index = 0; index < MAX_TASKS; index++){
//		if (SCH_Tasks[index].taskID == taskID) {
//			if (!SCH_Tasks[index].isUsed) return;
//			break;
//		}
//	}
//
//    if(index == MAX_TASKS) return;
//
//    uint32_t d = SCH_Tasks[index].Delay;
//    SCH_Tasks[index].isUsed = 0;
//
//    // Shift left to close gap
//    for(int i = index; i < MAX_TASKS - 1; i++)
//        SCH_Tasks[i] = SCH_Tasks[i+1];
//
//    SCH_Tasks[MAX_TASKS - 1].isUsed = 0;
//
//    // Fix delay
//    if(SCH_Tasks[index].isUsed)
//        SCH_Tasks[index].Delay += d;
//
//    RingBuffer_append(&taskID_buffer, taskID);
//}

int SCH_Delete_Task(uint16_t taskID, uint8_t give_back_taskID) {
    int index = -1;

    for (int i = 0; i < MAX_TASKS; ++i) {
        if (SCH_Tasks[i].isUsed && SCH_Tasks[i].taskID == taskID) {
            index = i;
            break;
        }
    }
    if (index == -1) return -1;

    uint32_t d = SCH_Tasks[index].Delay;

    if (index + 1 < MAX_TASKS && SCH_Tasks[index + 1].isUsed) {
        SCH_Tasks[index + 1].Delay += d;
    }

    for (int i = index; i < MAX_TASKS - 1; ++i) {
        SCH_Tasks[i] = SCH_Tasks[i + 1];
    }

    SCH_Tasks[MAX_TASKS - 1].isUsed = 0;
    SCH_Tasks[MAX_TASKS - 1].Delay = 0;
    SCH_Tasks[MAX_TASKS - 1].Period = 0;
    SCH_Tasks[MAX_TASKS - 1].pTask = NULL;
    SCH_Tasks[MAX_TASKS - 1].taskID = 0;

    if (give_back_taskID)
    	RingBuffer_append(&taskID_buffer, taskID);

    return 0;
}
void SCH_Update(void) {
    if(SCH_Tasks[0].isUsed && SCH_Tasks[0].Delay > 0)
        SCH_Tasks[0].Delay--;
}

void SCH_Dispatcher(void) {
    if(SCH_Tasks[0].isUsed && SCH_Tasks[0].Delay == 0) {
        (*SCH_Tasks[0].pTask)();

        if(SCH_Tasks[0].Period > 0) {
        	uint16_t taskID = SCH_Tasks[0].taskID;
            void (*taskFunc)() = SCH_Tasks[0].pTask;
            uint32_t per = SCH_Tasks[0].Period;
            SCH_Delete_Task(SCH_Tasks[0].taskID, 0);
            SCH_ReAdd_Task(taskFunc, per, per, taskID);
        }
        else {
            SCH_Delete_Task(SCH_Tasks[0].taskID, 1);
        }
    }
}

//void HAL_TIM_PeriodElapsedCallback(TIM_HandleTypeDef *htim) {
//    SCH_Update();
//}

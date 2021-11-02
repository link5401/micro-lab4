/*
 * scheduler.h
 *
 *  Created on: Oct 27, 2021
 *      Author: PC
 */

#ifndef INC_SCH_H_
#define INC_SCH_H_

#include "main.h"

typedef struct{
	//pointer to the next task
	void (*pTask)(void);
	//delay (ticks)
	uint32_t Delay;
	//Interval(ticks) between subsequent runs
	uint32_t Period;
	//Incremented (by scheduler when task is due to execute)
	uint8_t RunMe;

	uint32_t TaskID;
} sTask;

#define SCH_MAX_TASKS										40
#define NO_TASK_ID 											0

sTask SCH_tasks_G[SCH_MAX_TASKS];


void SCH_Init (void );

void SCH_Update(void );

unsigned char SCH_Add_Task(void (* pFunction)() , unsigned int DELAY, unsigned int PERIOD);

void SCH_Dispatch_Tasks(void );

unsigned char SCH_Delete_Task(const unsigned char TASK_INDEX);

void SCH_Go_To_Sleep(void );

void SCH_Report_Status(void);
#endif /* INC_SCH_H_ */

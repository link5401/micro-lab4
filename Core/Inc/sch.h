/*
 * scheduler.h
 *
 *  Created on: Oct 27, 2021
 *      Author: PC
 */

#ifndef INC_SCH_H_
#define INC_SCH_H_

#include "main.h"

void SCH_Init (void );

void SCH_Update(void );

unsigned char SCH_Add_Task(void (* pFunction)() , unsigned int DELAY, unsigned int PERIOD);

void SCH_Dispatch_Tasks(void );

unsigned char SCH_Delete_Task(const tByte TASK_INDEX);

void SCH_Go_To_Sleep(void );

void SCH_Report_Status(void);
#endif /* INC_SCH_H_ */

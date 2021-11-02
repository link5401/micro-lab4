/*
 * sch.c
 *
 *  Created on: Nov 2, 2021
 *      Author: PC
 */
#include "sch.h"
#include "main.h"
#define TIMER_CYCLE 10

//static unsigned char Last_error_code_G;
//static unsigned int Error_tick_count_G;
static unsigned char Error_code_G = 0;
void SCH_Init (void ){
	unsigned char i;
	for(i = 0; i< SCH_MAX_TASKS; i++){
		SCH_Delete_Task(i);
	}
	//Reset the global error variable
	//SCH_Delete_Task() will generate an error code
	Error_code_G = 0;
//	Timer_init();
//	Watchdog_init();

}

/////////////////////////////////////////////////////////////////////////////////////
/*   ISR FUNCTION
 * 	 What it does: determines if a task is due to run: Yes ? RunMe++.
 * 	 													No? RunMe--
 *
 *
 */
void SCH_Update(void ){
	unsigned char Index;
	for(Index = 0; Index < SCH_MAX_TASKS; Index++){
		if(SCH_tasks_G[Index].pTask){
			if(SCH_tasks_G[Index].Delay == 0){
				SCH_tasks_G[Index].RunMe += 1;
				if(SCH_tasks_G[Index].Period)
					SCH_tasks_G[Index].Delay = SCH_tasks_G[Index].Period;
			} else SCH_tasks_G[Index].Delay -=1;
		}
	}
}
void HAL_TIM_PeriodElapsedCallback (TIM_HandleTypeDef *htim ) {
	SCH_Update();
}
/////////////////////////////////////////////////////////////////////////////
/*
 *  ADD TASK FUNCTIONS
 */
unsigned char SCH_Add_Task(void (*pFunction)(), unsigned int DELAY, unsigned int PERIOD){
	unsigned char Index = 0;
	//FIND A GAP IN THE ARRAY OF TASKS
	while((SCH_tasks_G[Index].pTask != 0) && (Index < SCH_MAX_TASKS)) Index++;
	//NOGAP CASE
	if(Index == SCH_MAX_TASKS){
		Error_code_G = ERROR_SCH_TOO_MANY_TASKS;
		return SCH_MAX_TASKS;
	}
	//YESGAP CASE
	SCH_tasks_G[Index].pTask 	= pFunction;
	SCH_tasks_G[Index].Delay 	= DELAY/TIMER_CYCLE;
	SCH_tasks_G[Index].Period	= PERIOD/TIMER_CYCLE;
	SCH_tasks_G[Index].RunMe	= 0;
	return Index;
}
//////////////////////////////////////////////////////////////////////////////////////////
/*
 *  	DISPATCHER
 */
void SCH_Dispatch_Tasks(void){
	unsigned char Index;
	for(Index = 0; Index < SCH_MAX_TASKS; Index++){
		if(SCH_tasks_G[Index].RunMe > 0) {
			(*SCH_tasks_G[Index].pTask)(); // call function for the task
			SCH_tasks_G[Index].RunMe -=1; // reset flag
			//Periodic tasks will run again
			// if this is one shot task, delete it.
			if(SCH_tasks_G[Index].Period == 0) SCH_Delete_Task(Index);
		}
	}
	//SCH_Report_Status();
	//SCH_Go_To_Sleep();
}

unsigned char SCH_Delete_Task(const unsigned char TASK_INDEX){
	unsigned char Return_code;
	if(SCH_tasks_G[TASK_INDEX].pTask == 0 ){
		Error_code_G = ERROR_SCH_CANNOT_DELETE_TASK;
		   Return_code = Error_code_G;
	} else Return_code = 0;
	SCH_tasks_G[TASK_INDEX].pTask 	= 0x0000;
	SCH_tasks_G[TASK_INDEX].Delay 	= 0;
	SCH_tasks_G[TASK_INDEX].Period 	= 0;
	SCH_tasks_G[TASK_INDEX].RunMe	= 0;
	return Return_code;
}

// TODO
void SCH_Go_To_Sleep(){


}

void SCH_Report_Status(void){
#ifdef SCH_REPORT_ERRORS
	if(Error_code_G != Last_error_code_G)
		   if (Error_code_G != Last_error_code_G) {
		      // Negative logic on LEDs assumed
		      Error_port = 255 - Error_code_G;

		      Last_error_code_G = Error_code_G;

		      if (Error_code_G != 0) Error_tick_count_G = 60000;
		      else Error_tick_count_G = 0;
		   } else {
		      if (Error_tick_count_G != 0) {
		         if (--Error_tick_count_G == 0) {
		            Error_code_G = 0; // Reset error code
		         }
		      }
		    }
#endif
}


























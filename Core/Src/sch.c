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
 * 	 													No? Delay--
 *
 *
 */
uint8_t FIRST_INDEX = 0;
uint32_t TIME_STAMP = 0;
void SCH_Update(void ){

		if(SCH_tasks_G[FIRST_INDEX].pTask){
			if(SCH_tasks_G[FIRST_INDEX].Delay == 0){
				SCH_tasks_G[FIRST_INDEX].RunMe += 1;
//				if(SCH_tasks_G[FIRST_INDEX].Period)
//					SCH_tasks_G[FIRST_INDEX].Delay = SCH_tasks_G[FIRST_INDEX].Period;
			} else SCH_tasks_G[FIRST_INDEX].Delay -=1;
		}
		TIME_STAMP += 1;
}
void HAL_TIM_PeriodElapsedCallback (TIM_HandleTypeDef *htim ) {
	SCH_Update();

}
/////////////////////////////////////////////////////////////////////////////
/*
 *  ADD TASK FUNCTIONS
 */
void swapTask(sTask *t1, sTask *t2){
	void (*fTemp) = t1->pTask;
	t1->pTask = t2->pTask;
	t2->pTask = fTemp;

	uint32_t tempDelay = t1->Delay;
	t1->Delay = t2->Delay;
	t2->Delay = tempDelay;

	uint32_t tempPeriod = t1->Period;
	t1->Period = t2->Period;
	t2->Period = tempPeriod;

	uint8_t tempRun = t1->RunMe;
	t1->RunMe = t2->RunMe;
	t2->RunMe = tempRun;

}
uint16_t currentTaskNumber = 0;

uint32_t currentDelay = 0;

unsigned char SCH_Add_Task(void (*pFunction)(), unsigned int DELAY, unsigned int PERIOD){
//	unsigned char Index = 0;
//	//FIND A GAP IN THE ARRAY OF TASKS
//	while((SCH_tasks_G[Index].pTask != 0) && (Index < SCH_MAX_TASKS)) Index++;
//	//NOGAP CASE
//	if(Index == SCH_MAX_TASKS){
//		Error_code_G = ERROR_SCH_TOO_MANY_TASKS;
//		return SCH_MAX_TASKS;
//	}
//	//YESGAP CASE
//	SCH_tasks_G[Index].pTask 	= pFunction;
//	SCH_tasks_G[Index].Delay 	= DELAY/TIMER_CYCLE;
//	SCH_tasks_G[Index].Period	= PERIOD/TIMER_CYCLE;
//	SCH_tasks_G[Index].RunMe	= 0;
//	return Index;
/*
 * How this work:
 * 			Newly added task is added to the back of the array.
 * 			Then start traversing the array while(newlyadded.Delay >= totalDelay)
 * 			If the above condition is true, newlyadded.Delay -= totalDelay
 * 			Otherwise, swap it with the current index.
 * 			E.g:
 * 			[T1,2000,3000]
 *
 * 			*add(T2,5000,2000)
 * 			[T1,2000,7000][T2,3000,2000]
 *
 * 			*add(T3,3000,4000)
 * 			[T1,2000,7000][T3,1000,4000][T2,2000,2000]
 *
 *
 *
 */
	currentTaskNumber++;
	if(currentTaskNumber > SCH_MAX_TASKS) {
		Error_code_G = ERROR_SCH_TOO_MANY_TASKS;
		return SCH_MAX_TASKS;
	}
	if(!SCH_tasks_G[FIRST_INDEX].pTask){
			SCH_tasks_G[FIRST_INDEX].pTask 	= pFunction;
			SCH_tasks_G[FIRST_INDEX].Delay 	= DELAY/TIMER_CYCLE;
			SCH_tasks_G[FIRST_INDEX].Period	= PERIOD/TIMER_CYCLE;
			SCH_tasks_G[FIRST_INDEX].RunMe	= 0;


			return FIRST_INDEX;
	} else{
		uint8_t finished = 0;
		unsigned char index = currentTaskNumber - 1;
		SCH_tasks_G[index].pTask 	= pFunction;
		SCH_tasks_G[index].Delay 	= DELAY/TIMER_CYCLE;
		SCH_tasks_G[index].Period	= PERIOD/TIMER_CYCLE;
		SCH_tasks_G[index].RunMe	= 0;
		uint32_t totalDelay = currentDelay;
	while(!finished){
		int i = FIRST_INDEX;
		for(; i < currentTaskNumber - 1; i++){
			totalDelay+=SCH_tasks_G[i].Delay;
				if(SCH_tasks_G[index].Delay >= totalDelay) continue;
				else{
					SCH_tasks_G[index].Delay -= (totalDelay - SCH_tasks_G[i].Delay);
					SCH_tasks_G[i].Delay =  totalDelay;
					swapTask(&SCH_tasks_G[i],&SCH_tasks_G[index]);
					totalDelay = 0;
					break;

				}
		}
		if(i == currentTaskNumber - 1){
			SCH_tasks_G[index].Delay -= totalDelay;
			finished = 1;
		} else i = FIRST_INDEX;
		}
		return index;
	}


}
//////////////////////////////////////////////////////////////////////////////////////////
/*
 *  	DISPATCHER:
 *  		Check to see if FIRST_INDEX Delay is down to 0. If true, move FIRST_INDEX forward
 *  		in circular manner, adjust its delay.
 *
 */

void SCH_Dispatch_Tasks(void){
//	unsigned char Index;
//	for(Index = 0; Index < SCH_MAX_TASKS; Index++){
//		if(SCH_tasks_G[Index].RunMe > 0) {
//			(*SCH_tasks_G[Index].pTask)(); // call function for the task
//			SCH_tasks_G[Index].RunMe -=1; // reset flag
//			//Periodic tasks will run again
//			// if this is one shot task, delete it.
//			if(SCH_tasks_G[Index].Period == 0) SCH_Delete_Task(Index);
//		}
//	}
//	//SCH_Report_Status();
//	//SCH_Go_To_Sleep();

	 //check run me as usual
	 if(SCH_tasks_G[FIRST_INDEX].RunMe > 0) {
		 (*SCH_tasks_G[FIRST_INDEX].pTask)();
		 SCH_tasks_G[FIRST_INDEX].RunMe -=1;

		 uint32_t fPeriod = SCH_tasks_G[FIRST_INDEX].Period * TIMER_CYCLE;
		 void  *fTask = SCH_tasks_G[FIRST_INDEX].pTask;
		 currentDelay = TIME_STAMP;
		 SCH_Delete_Task(FIRST_INDEX);

		 SCH_Add_Task(fTask, (TIME_STAMP*TIMER_CYCLE) + fPeriod, fPeriod);
	 }
	 	//SCH_Report_Status();
	 	SCH_Go_To_Sleep();

}

unsigned char SCH_Delete_Task(const unsigned char TASK_INDEX){
	unsigned char Return_code;
	if(SCH_tasks_G[TASK_INDEX].pTask == 0 ){
		Error_code_G = ERROR_SCH_CANNOT_DELETE_TASK;
		   Return_code = Error_code_G;
	} else Return_code = 0;
	//delete by actually eliminating the index
	for(uint32_t i = TASK_INDEX; i < currentTaskNumber - 1; i++){
		SCH_tasks_G[i] = SCH_tasks_G[i+1];
	}
	currentTaskNumber -= 1;
	return Return_code;
}

// TODO
void SCH_Go_To_Sleep(){
	HAL_PWR_EnterSLEEPMode(PWR_LOWPOWERREGULATOR_ON, PWR_SLEEPENTRY_WFI);

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


























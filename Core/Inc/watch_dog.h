/*
 * watch_dog.h
 *
 *  Created on: Oct 27, 2021
 *      Author: PC
 */

#ifndef INC_WATCH_DOG_H_
#define INC_WATCH_DOG_H_

#include "main.h"

//void MX_IWDG_Init(void );

void WatchDog_Refresh(void );

unsigned char Is_Watchdog_Reset(void );

void WatchDog_Couting(void );

void Reset_WatchDog_Counting(void );

#endif /* INC_WATCH_DOG_H_ */

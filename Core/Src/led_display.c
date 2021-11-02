/*
 * led_display.c
 *
 *  Created on: Nov 2, 2021
 *      Author: PC
 */

#include "led_display.h"
#define NUMBER_OF_LED 	2

static GPIO_TypeDef * LEDPort[NUMBER_OF_LED] = {
		red_GPIO_Port,
		green_GPIO_Port
};

static uint16_t LEDPin[NUMBER_OF_LED] ={
		red_Pin,
		green_Pin
};

void led_init(){
	for(int i = 0; i < NUMBER_OF_LED; i++){
		HAL_GPIO_WritePin(LEDPort[i],LEDPin[i], 1);
	}
}

void toggle_red(){
	HAL_GPIO_TogglePin(LEDPort[0], LEDPin[0]);
}

void toggle_green(){
	HAL_GPIO_TogglePin(LEDPort[1], LEDPin[1]);
}

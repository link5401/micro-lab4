/*
 * segment_display.c
 *
 *  Created on: Oct 16, 2021
 *      Author: PC
 */


#include "main.h"
#include "segment_display.h"


//Maximum number of digits
#define NUMBER_OF_DECIMAL_DIGITS 	10
#define NUMBER_OF_SEVEN_SEGMENTS	1
#define NUMBER_OF_EDGES				7
/*LED CONVERSION
 * A is least significant bit
 * G is the most significant bit
 * E.g: 0x3f= 0b0111111 => LED displays 0*/
static uint8_t sevenSegmentLEDConversion[NUMBER_OF_DECIMAL_DIGITS] = {0x3f, 0x6, 0x5B, 0x4F, 0x66, 0x6D, 0x7D, 0x7, 0x7F, 0x6F};

// Seven segments LEDs

//Buffer variable
//ACTUAL ARRAY FOR DISPLAYING
static uint8_t segmentBuffer[NUMBER_OF_SEVEN_SEGMENTS] = {0};


//update segmentBuffer
uint8_t update_value_segment(uint8_t value, int idx)
{
	 if(idx >= NUMBER_OF_SEVEN_SEGMENTS) return 0;
	 if (value > NUMBER_OF_DECIMAL_DIGITS) return 0;
	 segmentBuffer[idx] = value;
	 return 1;
}


static GPIO_TypeDef * sevenSegmentDataPort[NUMBER_OF_EDGES] = {
		SEG0_GPIO_Port,
		SEG1_GPIO_Port,
		SEG2_GPIO_Port,
		SEG3_GPIO_Port,
		SEG4_GPIO_Port,
		SEG5_GPIO_Port,
		SEG6_GPIO_Port
};
static uint16_t sevenSegmentDataPin[NUMBER_OF_EDGES] = {
		SEG0_Pin,
		SEG1_Pin,
		SEG2_Pin,
		SEG3_Pin,
		SEG4_Pin,
		SEG5_Pin,
		SEG6_Pin
};
//get signals from segmentbuffer then writepin accordingly
void display_segment_number(int idx){
	uint8_t temp = sevenSegmentLEDConversion[segmentBuffer[idx]];
	//AND WITH EACH BIT IN 7 STARTING BITS
	uint8_t i;
	for(i = 0; i < NUMBER_OF_EDGES ; i++){
		if((temp>>i) & 0x01){
			HAL_GPIO_WritePin(sevenSegmentDataPort[i], sevenSegmentDataPin[i], 0);
		} else{
			HAL_GPIO_WritePin(sevenSegmentDataPort[i], sevenSegmentDataPin[i], 1);
		}
	}

}

uint16_t i = 0;
void display0to9(void ){
	if(i > 9) i = 0;
	update_value_segment(i++, 0);
	display_segment_number(0);
}
















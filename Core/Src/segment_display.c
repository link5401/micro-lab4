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
#define NUMBER_OF_SEVEN_SEGMENTS	4
#define NUMBER_OF_EDGES				7
/*LED CONVERSION
 * A is least significant bit
 * G is the most significant bit
 * E.g: 0x3f= 0b0111111 => LED displays 0*/
static uint8_t sevenSegmentLEDConversion[NUMBER_OF_DECIMAL_DIGITS] = {0x3f, 0x6, 0x5B, 0x4F, 0x66, 0x6D, 0x7D, 0x7, 0x7F, 0x6F};

// Seven segments LEDs

//Buffer variable
//ACTUAL ARRAY FOR DISPLAYING
static uint8_t segmentBuffer[NUMBER_OF_SEVEN_SEGMENTS];


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
	//A
//	if(temp & 0x01){
//		HAL_GPIO_WritePin(SEG0_GPIO_Port , SEG0_Pin , 0);
//	} else{
//		HAL_GPIO_WritePin(SEG0_GPIO_Port , SEG0_Pin , 1);
//	}
//	//B
//	if(temp & 0x02){
//		HAL_GPIO_WritePin(SEG1_GPIO_Port , SEG1_Pin , 0);
//	} else{
//		HAL_GPIO_WritePin(SEG1_GPIO_Port , SEG1_Pin , 1);
//	}
//	//C
//	if(temp & 0x04){
//		HAL_GPIO_WritePin(SEG2_GPIO_Port , SEG2_Pin , 0);
//	} else{
//		HAL_GPIO_WritePin(SEG2_GPIO_Port , SEG2_Pin , 1);
//	}
//	//D
//	if(temp & 0x08){
//		HAL_GPIO_WritePin(SEG3_GPIO_Port , SEG3_Pin , 0);
//	} else{
//		HAL_GPIO_WritePin(SEG3_GPIO_Port , SEG3_Pin , 1);
//	}
//	//E
//	if(temp & 0x10){
//		HAL_GPIO_WritePin(SEG4_GPIO_Port , SEG4_Pin , 0);
//	} else{
//		HAL_GPIO_WritePin(SEG4_GPIO_Port , SEG4_Pin , 1);
//	}
//	//F
//	if(temp & 0x20){
//		HAL_GPIO_WritePin(SEG5_GPIO_Port , SEG5_Pin , 0);
//	} else{
//		HAL_GPIO_WritePin(SEG5_GPIO_Port , SEG5_Pin , 1);
//	}
//	//G
//	if(temp & 0x40){
//		HAL_GPIO_WritePin(SEG6_GPIO_Port , SEG6_Pin , 0);
//	} else{
//		HAL_GPIO_WritePin(SEG6_GPIO_Port , SEG6_Pin , 1);
//	}
	uint8_t i;
	for(i = 0; i < NUMBER_OF_EDGES ; i++){
		if((temp>>i) & 0x01){
			HAL_GPIO_WritePin(sevenSegmentDataPort[i], sevenSegmentDataPin[i], 0);
		} else{
			HAL_GPIO_WritePin(sevenSegmentDataPort[i], sevenSegmentDataPin[i], 1);
		}
	}

}


//ARRAY FOR VALUES OF 4 LED
//THIS IS NOT THE ARRAY FOR DISPLAYING.
int led_buffer[4] = {0, 1, 0, 1};
int index_led = 0;
//update led_index and EN signals

void update7SEG(uint8_t index){
	switch(index){
		case 0:
			HAL_GPIO_WritePin(EN0_GPIO_Port, EN0_Pin, 0);
			HAL_GPIO_WritePin(EN1_GPIO_Port, EN1_Pin, 1);
			HAL_GPIO_WritePin(EN2_GPIO_Port, EN2_Pin, 1);
			HAL_GPIO_WritePin(EN3_GPIO_Port, EN3_Pin, 1);
			update_value_segment(led_buffer[index], index);
			display_segment_number(index);
			index_led = index;
			break;
		case 1:
			HAL_GPIO_WritePin(EN0_GPIO_Port, EN0_Pin, 1);
			HAL_GPIO_WritePin(EN1_GPIO_Port, EN1_Pin, 0);
			HAL_GPIO_WritePin(EN2_GPIO_Port, EN2_Pin, 1);
			HAL_GPIO_WritePin(EN3_GPIO_Port, EN3_Pin, 1);
			update_value_segment(led_buffer[index], index);
			display_segment_number(index);
			index_led = index;
			break;
		case 2:
			HAL_GPIO_WritePin(EN0_GPIO_Port, EN0_Pin, 1);
			HAL_GPIO_WritePin(EN1_GPIO_Port, EN1_Pin, 1);
			HAL_GPIO_WritePin(EN2_GPIO_Port, EN2_Pin, 0);
			HAL_GPIO_WritePin(EN3_GPIO_Port, EN3_Pin, 1);
			update_value_segment(led_buffer[index], index);
			display_segment_number(index);
			index_led = index;
			break;
		case 3:
			HAL_GPIO_WritePin(EN0_GPIO_Port, EN0_Pin, 1);
			HAL_GPIO_WritePin(EN1_GPIO_Port, EN1_Pin, 1);
			HAL_GPIO_WritePin(EN2_GPIO_Port, EN2_Pin, 1);
			HAL_GPIO_WritePin(EN3_GPIO_Port, EN3_Pin, 0);
			update_value_segment(led_buffer[index], index);
			display_segment_number(index);
			index_led = index;
			break;
		default:
			break;
	}
}
void clear7SEG(void){
	HAL_GPIO_WritePin(EN0_GPIO_Port, EN0_Pin, 1);
	HAL_GPIO_WritePin(EN1_GPIO_Port, EN1_Pin, 1);
	HAL_GPIO_WritePin(EN2_GPIO_Port, EN2_Pin, 1);
	HAL_GPIO_WritePin(EN3_GPIO_Port, EN3_Pin, 1);
}

//update led_buffer
void updateClockBuffer(int first, int second){
	if(first < 10){
		led_buffer[0] = 0;
		led_buffer[1] = first;
	} else{
		led_buffer[0] = first/10;
		led_buffer[1] = first%10;
	}

	if(second < 10){
		led_buffer[2] = 0;
		led_buffer[3] = second;
	} else {
		led_buffer[2] = second/10;
		led_buffer[3] = second%10;
	}
}




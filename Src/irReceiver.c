/*
 * irReceiver.c
 *
 *  Created on: 16 Oca 2022
 *      Author: alibl
 */

#include "irReceiver.h"

extern TIM_HandleTypeDef htim4;
#define TIMER		 	&htim4
#define TIM_CHANNEL 	TIM_CHANNEL_1

uint8_t cnt = 0;
uint32_t data = 0;
int32_t Difference = 0;
uint32_t IR_Data = 0;

int32_t IC_Val1 = 0, IC_Val2 = 0;
uint8_t Is_First_Captured = 0;
uint8_t cnt_Max = 67;
uint8_t mode = 0;
uint8_t latter = 0;

void IR_Start(void) {
	HAL_TIM_IC_Start_IT(TIMER, TIM_CHANNEL);
}

void HAL_TIM_IC_CaptureCallback(TIM_HandleTypeDef *htim) {
	IR_Receiver_IT(htim);
}

void IR_Receiver_IT(TIM_HandleTypeDef *htim) {
	if (Is_First_Captured == 0) {
		IC_Val1 = HAL_TIM_ReadCapturedValue(htim, TIM_CHANNEL_1); // read the first value
		Is_First_Captured = 1;  // set the first captured as true
	}

	else {
		IC_Val2 = HAL_TIM_ReadCapturedValue(htim, TIM_CHANNEL_1);  // read second value

		if (IC_Val2 > IC_Val1) {
			Difference = IC_Val2 - IC_Val1;
		}

		else if (IC_Val1 > IC_Val2) {
			Difference = (0xffff - IC_Val1) + IC_Val2;
		}

		if (mode) {
			if (latter) {
				if (Difference > 400 && Difference < 700) {
					//data = 0
					data |= (0 << (((cnt - 1) / 2) - 1));
				}
				else if (Difference > 1400 && Difference < 1800) {
					//data = 1
					data |= (1 << (((cnt - 1) / 2) - 1));
				}
				latter = 0;
			}
			else
				latter = 1;
		}

		if (Difference > 3500 && Difference < 5000) {
			mode = 1; //bir kez basma
			cnt_Max = 67;
		}
		else if (Difference > 1900 && Difference < 2500) {
			mode = 0; //basılı tutma
			cnt_Max = 3;
		}

		cnt++;

		if (cnt >= cnt_Max) {

			__HAL_TIM_SET_COUNTER(htim, 0);  // reset the counter
			Is_First_Captured = 0; // set it back to false

			if (mode) {
				IR_Data = data;
			}

			cnt = 0;
			latter = 0;
			mode = 0;

			data = 0;
		}

		IC_Val1 = IC_Val2;
	}
}

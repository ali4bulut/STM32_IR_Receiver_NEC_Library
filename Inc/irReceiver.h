/*
 * irReceiver.h
 *
 *  Created on: 16 Oca 2022
 *      Author: alibl
 */

#ifndef INC_IRRECEIVER_H_
#define INC_IRRECEIVER_H_

#include "stm32f4xx_hal.h"

//Herhangi bir timer'ı 1Mhz'e getir ve IC Interrupt modunda Rising and Falling edge trigger'ı seç.
//Seçtiğin timer'ı .c dosyasında da güncelle.

extern uint32_t IR_Data;

void IR_Start(void);

void IR_Receiver_IT(TIM_HandleTypeDef *htim);


#endif /* INC_IRRECEIVER_H_ */

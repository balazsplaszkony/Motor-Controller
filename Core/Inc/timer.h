/*
 * timer.h
 *
 *  Created on: May 13, 2023
 *      Author: plasz
 */

#ifndef INC_TIMER_H_
#define INC_TIMER_H_
#include "compontents.h"
#include "stm32f4xx.h"
#include "stm32f4xx_hal.h"
#include "stm32f4xx_hal_cortex.h"



// TIM7 inicializálása 100 us időzítőnek
void InitTimer();

// Megszakításkezelő a TIM7 időzítőhöz
void TIM7_IRQHandler(void);

void TimerHandler();
// Megszakításkezelő callback a Timer megszakításkezelőhöz

#endif /* INC_TIMER_H_ */

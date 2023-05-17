/*
 * timer.c
 *
 *  Created on: May 13, 2023
 *      Author: plasz
 */

#include "timer.h"

TIM_HandleTypeDef Tim7Handle;

// TIM7 inicializálása 100 us időzítőnek
void InitTimer()
{
	// APB1: 42 MHz (TIMx: 84 MHz)
	__TIM7_CLK_ENABLE();

	// TIM7: 10 kHz (TIM7_FREQ)
	Tim7Handle.Instance = TIM7;
	Tim7Handle.Init.ClockDivision = TIM_CLOCKDIVISION_DIV1;
	Tim7Handle.Init.CounterMode = TIM_COUNTERMODE_UP;
	Tim7Handle.Init.Prescaler = 0;
	Tim7Handle.Init.Period = 8399;
	Tim7Handle.State = HAL_TIM_STATE_RESET;

	HAL_TIM_Base_Init(&Tim7Handle);
	//HAL_TIM_Base_Start(&Tim7Handle);


	HAL_NVIC_SetPriority(TIM7_IRQn, 0, 0);
	HAL_NVIC_EnableIRQ(TIM7_IRQn);

	HAL_TIM_Base_Start_IT(&Tim7Handle);
}

// Megszakításkezelő a TIM7 időzítőhöz
void TIM7_IRQHandler(void)
{
	HAL_TIM_IRQHandler(&Tim7Handle);
}



void TimerHandler()
{
	int32_t measurement = GetQEPValue();

	if(isCharacteristicUpdated())//ez itt nem jó, inkább vizsáljuk majd jött-e üzenet
	{
		//if(prevbase != currentbase)
		//itt megvan még minden adatunk, mentsük lee
		//	elösször álljunk ba a basere
		 //uint32_t prev_baseRPM = characteristic.baseRPM ;
		 if((characteristic.baseRPM - measurement) > 3)
			 PID.setpoint = characteristic.baseRPM;

		 else{
				//EZT MAJD VISSZAKELL ÍRNI KOMMENT NÉLKÜL
				//SetCharacteristic(max, base, rise, fall, hold);
			 //		PID.setpoint = CalculateSetPoint(1);
		 }


	}
	else{
//		if(prev_baseRPM == characteristic.baseRPM)
//
		PID.setpoint = CalculateSetPoint(0);
	}


	PIDContollerUpdate(measurement);

	SetPWM(PID.output);

	DisplayValue(measurement);

}
// Megszakításkezelő callback a Timer megszakításkezelőhöz
void HAL_TIM_PeriodElapsedCallback(TIM_HandleTypeDef * htim)
{
    if (htim->Instance == TIM7)
    {
        TimerHandler();
    }
}

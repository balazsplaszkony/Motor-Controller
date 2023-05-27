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
	Tim7Handle.Init.Prescaler = 8399;
	Tim7Handle.Init.Period = 1000;
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
	static int i = 0;
	static int a = 0;
	static int reset = 1;
	static int last_measurement = 0;
//	i++;
//	DisplayValue(i);
//

//	if(i == 999)
//	{
//		a++;
//		DisplayValue(a);
//	}
//	i++;
//	i = i % 1000;


	int32_t measurement = HallEncoder_GetRPM();
	if(fabs(measurement-last_measurement) > measurement && last_measurement != 0)
		measurement = last_measurement;

	DisplayValue(measurement);
	if(reset)
	{
		PID.setpoint = CalculateSetPoint(reset);
		reset = 0;
	}
	else{
		PID.setpoint = CalculateSetPoint(reset);
	}

		PIDContollerUpdate(measurement);
		SetPWM(PID.output);
	last_measurement = measurement;

//	static int a = 0;
//	static int i = 0;
//
//
////	if(abs((int)measurement) > 1000)
////	{
////		;
////	}
////	if(a == 0)
////	{
////		i++;
////		DisplayValue(i);
////	}
//		//DisplayValue(measurement);
//
//	if(characteristic_new.updated)
//	{
//
//		//if(prevbase != currentbase)
//		//itt megvan még minden adatunk, mentsük lee
//		//	elösször álljunk ba a basere
//		 //uint32_t prev_baseRPM = characteristic.baseRPM ;
//		 if((characteristic.baseRPM - measurement) > 3)
//			 PID.setpoint = characteristic_new.baseRPM;
//
//		 else{
//				//EZT MAJD VISSZAKELL ÍRNI KOMMENT NÉLKÜL
//			 characteristic = characteristic_new;
//			 PID.setpoint = CalculateSetPoint(1, measurement);
//		 }
//	}
//	else{
//		PID.setpoint = CalculateSetPoint(0, measurement);
//	}
//
//
//	PIDContollerUpdate(measurement);
//	a++;
//	a = a % 100;
//	i = i% 100;
//	//SetPWM(PID.output);


}
// Megszakításkezelő callback a Timer megszakításkezelőhöz
void HAL_TIM_PeriodElapsedCallback(TIM_HandleTypeDef * htim)
{
    if (htim->Instance == TIM7)
    {
        TimerHandler();
    }
}

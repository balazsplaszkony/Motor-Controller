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
	static bool reset = true;
	static float last_measurement = 0.0;
	if(isCharacteristicUpdated(&characteristic_new))
	{
		characteristic = characteristic_new;
		reset = 1;
	}

	float measurement = HallEncoder_GetRPM();


	if(measurement >= -0.0001 && measurement <= 0.0001)
	{
		measurement = 0.0;
		last_measurement = 0.0;
	}

	//This needs to be added because there are spikes in the encoder's signal
	else if(fabs(measurement-last_measurement) > measurement && last_measurement != 0.0)
		measurement = last_measurement;

	DisplayValue(measurement);

	PID.setpoint = CalculateSetPoint(reset, measurement);

	if(reset)
		reset = false;

	PIDContollerUpdate(measurement);
	SetPWM(PID.output);
	last_measurement = measurement;
}
// Megszakításkezelő callback a Timer megszakításkezelőhöz
void HAL_TIM_PeriodElapsedCallback(TIM_HandleTypeDef * htim)
{
    if (htim->Instance == TIM7)
    {
        TimerHandler();
    }
}

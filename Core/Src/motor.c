/*
 * motor.c
 *
 *  Created on: May 13, 2023
 *      Author: plasz
 */

#include "motor.h"
#include "stm32f4xx.h"
#include "stm32f4xx_hal.h"
#include "stm32f4xx_hal_cortex.h"
TIM_HandleTypeDef Tim3Handle;

// TIM1 inicializálása PWM üzemmódban
void InitMotorPWM()
{
	// APB2: 42 MHz (TIM3: 84 MHz)
	__TIM1_CLK_ENABLE();

	// Timer1 konfigurációja PWM üzemmódban a ledek  alapján 21 kHz-re

	// HAL_TIM_Base_Init és HAL_TIM_PWM_Init hívása a megfelelően kitöltött TIM_HandleTypeDef struktúrával
	Tim3Handle.Instance = TIM3;
	Tim3Handle.Init.ClockDivision = TIM_CLOCKDIVISION_DIV1;
	Tim3Handle.Init.CounterMode = TIM_COUNTERMODE_UP;
	Tim3Handle.Init.Prescaler = 3;
	Tim3Handle.Init.Period = MOTOR_D_MAX;
	Tim3Handle.State = HAL_TIM_STATE_RESET;

	HAL_TIM_Base_Init(&Tim3Handle);
	HAL_TIM_PWM_Init(&Tim3Handle);
	TIM_OC_InitTypeDef TIM_OCInitStructure;
	// HAL_TIM_PWM_ConfigChannel hívása a megfelelően kitöltött TIM_OC_InitTypeDef struktúrával 2 csatornára

	TIM_OCInitStructure.OCMode = TIM_OCMODE_PWM1;
	TIM_OCInitStructure.Pulse = 0;
	TIM_OCInitStructure.OCPolarity = TIM_OCPOLARITY_HIGH;
	TIM_OCInitStructure.OCNPolarity = TIM_OCNPOLARITY_LOW;
	TIM_OCInitStructure.OCFastMode = TIM_OCFAST_DISABLE;
	TIM_OCInitStructure.OCIdleState = TIM_OCIDLESTATE_RESET;
	TIM_OCInitStructure.OCNIdleState = TIM_OCNIDLESTATE_SET;

	HAL_TIM_PWM_ConfigChannel(&Tim3Handle, &TIM_OCInitStructure, TIM_CHANNEL_1);
	TIM_OCInitStructure.OCPolarity = TIM_OCPOLARITY_LOW;
	HAL_TIM_PWM_ConfigChannel(&Tim3Handle, &TIM_OCInitStructure, TIM_CHANNEL_2);


	HAL_TIM_PWM_Start(&Tim3Handle, TIM_CHANNEL_1);
	HAL_TIM_PWM_Start(&Tim3Handle, TIM_CHANNEL_2);
}

void SetPWM(int32_t d)
{
	if (d > MOTOR_D_MAX)
	{
		d=MOTOR_D_MAX;
	}
	else if (d < 0)
	{
		d=0;
	}

	// PWM csatornák beállítása SetMotorCh1 és SetMotorCh2 hívásával
	SetMotorCh1(d);
	SetMotorCh2(d);
}

void SetMotorCh1(uint16_t d)
{
	Tim3Handle.Instance->CCR1 = d;
}

void SetMotorCh2(uint16_t d)
{
	Tim3Handle.Instance->CCR2 = d;
}

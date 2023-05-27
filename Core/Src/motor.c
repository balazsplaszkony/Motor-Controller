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
TIM_OC_InitTypeDef sConfigOC_PWM_Channel1;
TIM_OC_InitTypeDef sConfigOC_PWM_Channel2;
// TIM1 inicializálása PWM üzemmódban
void InitMotorPWM()
{
	  // Enable GPIO clocks
	  __HAL_RCC_GPIOA_CLK_ENABLE();
	  __HAL_RCC_GPIOB_CLK_ENABLE();

	  // Configure GPIO pins as alternate functions
	  GPIO_InitTypeDef GPIO_InitStruct;
	  GPIO_InitStruct.Mode = GPIO_MODE_AF_PP;
	  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_HIGH;
	  GPIO_InitStruct.Pull = GPIO_NOPULL;

	  GPIO_InitStruct.Pin = GPIO_PIN_6;  // Example pin for channel 1
	  GPIO_InitStruct.Alternate = GPIO_AF2_TIM3;  // Example timer and alternate function
	  HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);

	  GPIO_InitStruct.Pin = GPIO_PIN_7;  // Example pin for channel 2
	  GPIO_InitStruct.Alternate = GPIO_AF2_TIM3;  // Example timer and alternate function
	  HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);

	  // Enable timer clock
	  __HAL_RCC_TIM3_CLK_ENABLE();

	  // Configure timer
	  Tim3Handle.Instance = TIM3;  // Example timer
	  Tim3Handle.Init.Prescaler = 3;
	  Tim3Handle.Init.CounterMode = TIM_COUNTERMODE_UP;
	  Tim3Handle.Init.Period = 1000;  // Example period (1kHz frequency)
	  Tim3Handle.Init.ClockDivision = TIM_CLOCKDIVISION_DIV1;
	  Tim3Handle.Init.AutoReloadPreload = TIM_AUTORELOAD_PRELOAD_ENABLE;
	  HAL_TIM_PWM_Init(&Tim3Handle);

	  // Configure PWM channels
	  sConfigOC_PWM_Channel1.OCMode = TIM_OCMODE_PWM1;
	  sConfigOC_PWM_Channel1.Pulse = 500;  // Example duty cycle (50%)
	  sConfigOC_PWM_Channel1.OCPolarity = TIM_OCPOLARITY_HIGH;
	  sConfigOC_PWM_Channel1.OCFastMode = TIM_OCFAST_DISABLE;
	  sConfigOC_PWM_Channel1.OCIdleState = TIM_OCIDLESTATE_RESET;
	  sConfigOC_PWM_Channel1.OCNIdleState = TIM_OCNIDLESTATE_SET;

	  HAL_TIM_PWM_ConfigChannel(&Tim3Handle, &sConfigOC_PWM_Channel1, TIM_CHANNEL_1);  // Example channel 1

	  // Update the polarity for channel 2
	  sConfigOC_PWM_Channel2.OCMode = TIM_OCMODE_PWM1;
	  sConfigOC_PWM_Channel2.Pulse = 500;  // Example duty cycle (50%)
	  sConfigOC_PWM_Channel2.OCPolarity = TIM_OCPOLARITY_LOW;
	  sConfigOC_PWM_Channel2.OCFastMode = TIM_OCFAST_DISABLE;
	  sConfigOC_PWM_Channel2.OCIdleState = TIM_OCIDLESTATE_RESET;
	  sConfigOC_PWM_Channel2.OCNIdleState = TIM_OCNIDLESTATE_SET;

	  HAL_TIM_PWM_ConfigChannel(&Tim3Handle, &sConfigOC_PWM_Channel2, TIM_CHANNEL_2);  // Example channel 2

	  // Start PWM generation
	  HAL_TIM_PWM_Start(&Tim3Handle, TIM_CHANNEL_1);  // Example channel 1
	  HAL_TIM_PWM_Start(&Tim3Handle, TIM_CHANNEL_2);  // Example channel 2
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

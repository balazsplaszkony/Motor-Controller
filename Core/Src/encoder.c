/*
 * encoder.c
 *
 *  Created on: May 13, 2023
 *      Author: plasz
 */


#include "encoder.h"

TIM_HandleTypeDef Tim1Handle;
int32_t prevCounterValue = 0;
float rpm = 0;

// TIM3 inicializálása QEP üzemmódban
void InitEncoder()
{


	  // Initialize GPIO and Timer 1 for encoder interface

	  // Configure GPIO pins for encoder signals
	  GPIO_InitTypeDef GPIO_InitStruct;
	  GPIO_InitStruct.Pin = GPIO_PIN_8 | GPIO_PIN_9;
	  GPIO_InitStruct.Mode = GPIO_MODE_AF_PP;
	  GPIO_InitStruct.Pull = GPIO_PULLUP;
	  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_HIGH;
	  GPIO_InitStruct.Alternate = GPIO_AF1_TIM1;
	  HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);

		// APB1: 84 MHz (TIMx: 168 MHz)
		__TIM1_CLK_ENABLE();

	  // Configure Timer 1 for encoder mode
	  Tim1Handle.Instance = TIM1;
	  Tim1Handle.Init.Prescaler = 0;
	  Tim1Handle.Init.CounterMode = TIM_COUNTERMODE_UP;
	  Tim1Handle.Init.Period = 0xFFFF;
	  Tim1Handle.Init.ClockDivision = TIM_CLOCKDIVISION_DIV1;
	  Tim1Handle.Init.RepetitionCounter = 0;
	  Tim1Handle.Init.AutoReloadPreload = TIM_AUTORELOAD_PRELOAD_DISABLE;

	  // Configure encoder interface mode
	  TIM_Encoder_InitTypeDef encoderConfig;
	  encoderConfig.EncoderMode = TIM_ENCODERMODE_TI12;
	  encoderConfig.IC1Polarity = TIM_ICPOLARITY_RISING;
	  encoderConfig.IC1Selection = TIM_ICSELECTION_DIRECTTI;
	  encoderConfig.IC1Prescaler = TIM_ICPSC_DIV1;
	  encoderConfig.IC1Filter = 0;
	  encoderConfig.IC2Polarity = TIM_ICPOLARITY_RISING;
	  encoderConfig.IC2Selection = TIM_ICSELECTION_DIRECTTI;
	  encoderConfig.IC2Prescaler = TIM_ICPSC_DIV1;
	  encoderConfig.IC2Filter = 0;

	  HAL_TIM_Encoder_Init(&Tim1Handle, &encoderConfig);
	  HAL_TIM_Encoder_Start(&Tim1Handle, TIM_CHANNEL_ALL);
}

// Enkóder érték kiolvasása
//int16_t GetEncoderValue()
//{
//	return (uint16_t)HAL_TIM_ReadCapturedValue(&Tim1Handle, TIM_CHANNEL_1);

//    static uint16_t previousCount = 0;
//    static uint32_t previousTime = 0;
//
//    uint16_t currentCount = (uint16_t)Tim1Handle.Instance->CNT;
//    uint32_t currentTime = HAL_GetTick();
//
//    uint16_t countDiff = currentCount - previousCount;
//    uint32_t timeDiff = currentTime - previousTime;
//
//    float rotations = (float)countDiff / (MOTOR_RES_ENC * 4.0f); // Number of rotations since the previous reading
//    float elapsedMinutes = (float)timeDiff / (1000.0f * 60.0f); // Elapsed time in minutes
//
//    float rpm = rotations / elapsedMinutes; // Rotations per minute
//
//    previousCount = currentCount;
//    previousTime = currentTime;
//
//    return (uint16_t)rpm;
//
//}

int32_t HallEncoder_GetRPM()
{
  int32_t counterValue = __HAL_TIM_GET_COUNTER(&Tim1Handle);

  // Calculate delta counter value
  int32_t deltaCounter = counterValue - prevCounterValue;

  // Update previous counter value
  prevCounterValue = counterValue;

  // Calculate RPM based on the resolution and time interval (100 microseconds)
  //rpm = (deltaCounter * 60  ) / 16*1000;
  rpm = deltaCounter* 10000/(2096*60);
  return rpm;
}

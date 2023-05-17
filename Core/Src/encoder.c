/*
 * encoder.c
 *
 *  Created on: May 13, 2023
 *      Author: plasz
 */


#include "encoder.h"

TIM_HandleTypeDef Tim1Handle;

// TIM3 inicializálása QEP üzemmódban
void InitQEP()
{
	// APB1: 84 MHz (TIMx: 168 MHz)
	__TIM1_CLK_ENABLE();

	// Timer3 konfigurációja enkóder módban

	// HAL_TIM_Base_Init hívása a megfelelően kitöltött TIM_HandleTypeDef struktúrával,
	// 1 teljes motorfordulat alatt MOTOR_RES_ENC inkrement érkezik
	Tim1Handle.Init.ClockDivision = TIM_CLOCKDIVISION_DIV1;
	Tim1Handle.Init.CounterMode = TIM_COUNTERMODE_UP;

	Tim1Handle.Init.Prescaler = 1;
	Tim1Handle.Init.Period = 2000;
	

	Tim1Handle.State = HAL_TIM_STATE_RESET;
	Tim1Handle.Instance = TIM1;
	HAL_TIM_Base_Init(&Tim1Handle);
	
	// HAL_TIM_Encoder_Init hív�sa a megfelelően kitöltött TIM_Encoder_InitTypeDef struktúrával,
	// 2 csatornára, 4-szeres kiértékeléssel, előosztás nélkül
	TIM_Encoder_InitTypeDef TIM_Encoder_InitStructure;
	
	TIM_Encoder_InitStructure.EncoderMode = TIM_ENCODERMODE_TI12;
	TIM_Encoder_InitStructure.IC1Prescaler = 0;
	TIM_Encoder_InitStructure.IC2Prescaler = 0;

	TIM_Encoder_InitStructure.IC1Filter = 0;
	TIM_Encoder_InitStructure.IC1Selection = TIM_ICSELECTION_DIRECTTI;
	TIM_Encoder_InitStructure.IC1Polarity = TIM_ICPOLARITY_RISING;

	TIM_Encoder_InitStructure.IC2Filter = 0;
	TIM_Encoder_InitStructure.IC2Selection = TIM_ICSELECTION_DIRECTTI;
	TIM_Encoder_InitStructure.IC2Polarity = TIM_ICPOLARITY_RISING;

	HAL_TIM_Encoder_Init(&Tim1Handle, &TIM_Encoder_InitStructure);

	HAL_TIM_Encoder_Start(&Tim1Handle, TIM_CHANNEL_1);
	HAL_TIM_Encoder_Start(&Tim1Handle, TIM_CHANNEL_2);
}

// Enkóder érték kiolvasása
uint16_t GetQEPValue()
{
//	return (uint16_t)HAL_TIM_ReadCapturedValue(&Tim1Handle, TIM_CHANNEL_1);

    static uint16_t previousCount = 0;
    static uint32_t previousTime = 0;

    uint16_t currentCount = (uint16_t)Tim1Handle.Instance->CNT;
    uint32_t currentTime = HAL_GetTick();

    uint16_t countDiff = currentCount - previousCount;
    uint32_t timeDiff = currentTime - previousTime;

    float rotations = (float)countDiff / (MOTOR_RES_ENC * 4.0f); // Number of rotations since the previous reading
    float elapsedMinutes = (float)timeDiff / (1000.0f * 60.0f); // Elapsed time in minutes

    float rpm = rotations / elapsedMinutes; // Rotations per minute

    previousCount = currentCount;
    previousTime = currentTime;

    return (uint16_t)rpm;
}

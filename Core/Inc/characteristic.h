/*
 * characteristic.h
 *
 *  Created on: May 12, 2023
 *      Author: plasz
 */

#ifndef SRC_CHARACTERISTIC_H_
#define SRC_CHARACTERISTIC_H_
#include <stdint.h>
#include <stdbool.h>
#include "compontents.h"

#define MOTOR_TIM7_FREQ				10000		// 100 us timer IT


typedef struct Characteristic{
	uint32_t maximum_RPM;
	uint32_t baseRPM;
	uint32_t rise_time;
	uint32_t fall_time;
	uint32_t hold_time;
	uint32_t total_time;
	uint32_t delta_rising;
	uint32_t delta_falling;

	bool updated;
}Characteristic;

extern volatile Characteristic characteristic;

void CharacteristicInit();
void SetCharacteristic(uint32_t max, uint32_t base, uint32_t rise, uint32_t fall, uint32_t hold);
uint32_t GetTotalTime();
uint32_t GetDeltaRising();
uint32_t GetDeltaFalling();


bool isCharacteristicUpdated();
uint32_t CalculateSetPoint(bool reset);

#endif /* SRC_CHARACTERISTIC_H_ */
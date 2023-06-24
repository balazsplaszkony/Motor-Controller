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
#include <math.h>
#include "compontents.h"

#define PID_FREQ 10.0

typedef struct Characteristic {
	float maximum_RPM;
	float baseRPM;
	float rise_time;
	float fall_time;
	float hold_time;
	float total_time;
	float delta_rising;
	float delta_falling;

	bool updated;
	bool startable;
}Characteristic;

extern Characteristic characteristic;
extern Characteristic characteristic_new;


void CharacteristicInit();
void SetCharacteristic(Characteristic* characteristic, uint32_t max, uint32_t base, uint32_t rise, uint32_t fall, uint32_t hold);
float GetTotalTime(Characteristic* characteristic);
float GetDeltaRising(Characteristic* characteristic);
float GetDeltaFalling(Characteristic* characteristic);
bool hasNewData();

bool isCharacteristicUpdated(Characteristic* characteristic);
float CalculateSetPoint(bool reset, float measurement);

#endif /* SRC_CHARACTERISTIC_H_ */

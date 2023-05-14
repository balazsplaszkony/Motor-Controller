/*
 * characteristic.c
 *
 *  Created on: May 12, 2023
 *      Author: plasz
 */

#include "characteristic.h"

volatile Characteristic characteristic;

void CharacteristicInit()
{
	characteristic.maximum_RPM = 0;
	characteristic.baseRPM = 0;
	characteristic.rise_time = 0;
	characteristic.fall_time = 0;
	characteristic.hold_time = 0;
	characteristic.total_time = 0;
	characteristic.delta_rising = 0;
	characteristic.delta_falling = 0;
	characteristic.updated = false;
}

void SetCharacteristic(uint32_t max, uint32_t base, uint32_t rise, uint32_t fall, uint32_t hold)
{
	characteristic.maximum_RPM = max;
	characteristic.baseRPM = base;

	characteristic.rise_time = rise;
	characteristic.fall_time = fall;
	characteristic.hold_time = hold;

	characteristic.total_time = GetTotalTime();
	characteristic.delta_rising = GetDeltaRising();
	characteristic.delta_falling = GetDeltaFalling();

	characteristic.updated = true;
}

uint32_t GetTotalTime()
{
	return characteristic.rise_time + characteristic.fall_time + characteristic.hold_time;
}
uint32_t GetDeltaRising()
{
	return (characteristic.maximum_RPM - characteristic.baseRPM) / characteristic.delta_rising;
}

uint32_t GetDeltaFalling()
{
	return (characteristic.maximum_RPM - characteristic.baseRPM) / characteristic.delta_falling;
}

bool isCharacteristicUpdated()
{
	bool retval = characteristic.updated;
	if(characteristic.updated)
		characteristic.updated = !characteristic.updated;

	return retval;
}

uint32_t CalculateSetPoint(bool reset)
{
	static uint32_t counter = 0;
	static uint32_t setpoint = 0;
	if(reset)
	{
		counter = 0;
		setpoint = characteristic.baseRPM;
	}

	else
		counter = (counter + 1) % (MOTOR_TIM7_FREQ * GetTotalTime());

	if(counter < MOTOR_TIM7_FREQ * characteristic.rise_time)
		setpoint += characteristic.delta_rising;

	else if(counter > MOTOR_TIM7_FREQ * characteristic.rise_time &&
			counter < MOTOR_TIM7_FREQ * characteristic.rise_time + characteristic.hold_time)
	{
		;
	}

	else if((counter > MOTOR_TIM7_FREQ * (characteristic.rise_time + characteristic.hold_time)) &&
			(setpoint >= characteristic.baseRPM))
		setpoint -= characteristic.delta_falling;

	return setpoint;
}

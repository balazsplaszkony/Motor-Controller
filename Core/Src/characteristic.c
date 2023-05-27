/*
 * characteristic.c
 *
 *  Created on: May 12, 2023
 *      Author: plasz
 */

#include "characteristic.h"

volatile Characteristic characteristic;
volatile Characteristic characteristic_new;

void CharacteristicInit()
{
	characteristic.maximum_RPM = 30;
	characteristic.baseRPM = 0;
	characteristic.rise_time = 2;
	characteristic.fall_time = 2;
	characteristic.hold_time = 5;
	characteristic.total_time = GetTotalTime();
	characteristic.delta_rising = GetDeltaRising();
	characteristic.delta_falling = GetDeltaFalling();
	characteristic.updated = false;
	characteristic.startable = false;
}

void SetCharacteristic(Characteristic* characteristic ,uint32_t max, uint32_t base, uint32_t rise, uint32_t fall, uint32_t hold)
{
	characteristic->maximum_RPM = max;
	characteristic->baseRPM = base;

	characteristic->rise_time = rise;
	characteristic->fall_time = fall;
	characteristic->hold_time = hold;

	characteristic->total_time = GetTotalTime();
	characteristic->delta_rising = GetDeltaRising();
	characteristic->delta_falling = GetDeltaFalling();

	characteristic->updated = true;
}

uint32_t GetTotalTime()
{
	return characteristic.rise_time + characteristic.fall_time + characteristic.hold_time;
}
uint32_t GetDeltaRising()
{
	return (characteristic.maximum_RPM - characteristic.baseRPM) / characteristic.rise_time;
}

uint32_t GetDeltaFalling()
{
	return (characteristic.maximum_RPM - characteristic.baseRPM) / characteristic.fall_time;
}

bool isCharacteristicUpdated()
{
	bool retval = characteristic.updated;
	if(characteristic.updated)
		characteristic.updated = !characteristic.updated;

	return retval;
}

int32_t CalculateSetPoint(bool reset)
{
	static uint32_t counter = 0;
	static float setpoint = 0;

	if(reset || counter == 0)
	{
		counter = 0;
		setpoint = characteristic.baseRPM;
	}

	else
	{
//		if(abs(setpoint - measurement) > setpoint * 0.05)
//			return setpoint;
//
//		uint32_t totalTime = GetTotalTime();
//		 if (totalTime != 0)  // Check if totalTime is non-zero
//		 {
//	            counter = (counter + 1) % (MOTOR_TIM7_FREQ * totalTime);
//		 }
//		 else
//			 counter = 0;
		if(counter <= PID_FREQ * characteristic.rise_time)
				setpoint += (characteristic.delta_rising/PID_FREQ);

			else if(counter > PID_FREQ * characteristic.rise_time &&
					counter <= PID_FREQ * (characteristic.rise_time + characteristic.hold_time))
			{
				;
			}

			else if((counter > PID_FREQ * (characteristic.rise_time + characteristic.hold_time)) &&
					(counter <= GetTotalTime() * PID_FREQ))
						setpoint -= (characteristic.delta_falling/PID_FREQ);

	}
	counter++;
	counter = counter % ((uint32_t)PID_FREQ* GetTotalTime());
	return setpoint;

}

bool hasNewData(){
	;
}


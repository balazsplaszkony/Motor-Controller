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
	characteristic.maximum_RPM = 69;
	characteristic.baseRPM = 50;
	characteristic.rise_time = 10;
	characteristic.fall_time = 1;
	characteristic.hold_time = 5;
	characteristic.total_time = GetTotalTime(&characteristic);
	characteristic.delta_rising = GetDeltaRising(&characteristic);
	characteristic.delta_falling = GetDeltaFalling(&characteristic);
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

	characteristic->total_time = rise + fall + hold;
	characteristic->delta_rising = (max- base) / rise;
	characteristic->delta_falling = (max-base)/fall;

	characteristic->updated = true;
}

float GetTotalTime(Characteristic* characteristic)
{
	return (characteristic->rise_time + characteristic->fall_time + characteristic->hold_time);
}
float GetDeltaRising(Characteristic* characteristic)
{
	return ((characteristic->maximum_RPM - characteristic->baseRPM) / characteristic->rise_time);
}

float GetDeltaFalling(Characteristic* characteristic)
{
	return ((characteristic->maximum_RPM - characteristic->baseRPM) / characteristic->fall_time);
}

bool isCharacteristicUpdated(Characteristic* characteristic)
{
	bool retval = characteristic->updated;
	if(characteristic->updated)
		characteristic->updated = !characteristic->updated;

	return retval;
}

float CalculateSetPoint(bool reset, float measurement)
{
	static uint32_t counter = 0;
	static float setpoint = 0;
	static bool base_rpm_flag = false;

	if(reset || counter == 0 || !base_rpm_flag)
	{
		if(reset)
			base_rpm_flag = false;
		counter = 0;
		setpoint = characteristic.baseRPM;
		//if(characteristic.baseRPM)
		if(fabs(characteristic.baseRPM - measurement) < 5)
		base_rpm_flag = true;
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
					(counter <= GetTotalTime(&characteristic) * PID_FREQ))
						setpoint -= (characteristic.delta_falling/PID_FREQ);

	}
	counter++;
	counter = counter % ((uint32_t)PID_FREQ* (uint32_t)GetTotalTime(&characteristic));
	return setpoint;

}

bool hasNewData(){
	;
}

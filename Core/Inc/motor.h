/*
 * motor.h
 *
 *  Created on: May 13, 2023
 *      Author: plasz
 */

#ifndef INC_MOTOR_H_
#define INC_MOTOR_H_



#endif /* INC_MOTOR_H_ */
#include "compontents.h"
#define MOTOR_D_MAX					1000		// Motor PWM max. kitöltési tényező


void InitMotorPWM();
void SetPWM(int32_t d);
void SetMotorCh1(uint16_t d);
void SetMotorCh2(uint16_t d);

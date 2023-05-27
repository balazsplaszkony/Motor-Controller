/*
 * encoder.h
 *
 *  Created on: May 13, 2023
 *      Author: plasz
 */

#ifndef INC_ENCODER_H_
#define INC_ENCODER_H_

//#define MOTOR_RES_ENC					16
#define ENCODER_RESOLUTION 				2096
#include "main.h"

void InitEncoder();
int32_t HallEncoder_GetRPM();
#endif /* INC_ENCODER_H_ */

extern float rpm;

/*
 * compontents.h
 *
 *  Created on: May 13, 2023
 *      Author: plasz
 */

#ifndef INC_COMPONTENTS_H_
#define INC_COMPONTENTS_H_

#include "stm32f4xx_hal.h"
#include <math.h>

#include "PID.h"
#include "characteristic.h"
#include "display.h"
#include "encoder.h"
#include "motor.h"
#include "cmsis_gcc.h"
#include "timer.h"

void enterCriticalSection();

void exitCriticalSection();
#endif /* INC_COMPONTENTS_H_ */

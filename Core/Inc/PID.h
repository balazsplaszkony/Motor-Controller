/*
 * PID.h
 *
 *  Created on: May 12, 2023
 *      Author: plasz
 */

#ifndef SRC_PID_H_
#define SRC_PID_H_

#include <stdbool.h>
#include <math.h>
#include <stdint.h>
#include "compontents.h"

#define MAX_PID_OUTPUT 1000
#define MIN_PID_OUTPUT 0
#define PID_OUTPUT_OFFSET 500
#define MAX_RPM 86 //üresjárásban ennyi


typedef struct PidController{
    float Kp;
    float Ki;
    float Kd;
    float setpoint;
    float integral;
    float last_error;
    uint32_t output;
    bool updated;
//    float min_output;
//    float max_output;
//    float output_offset;
} PidController;

extern volatile PidController PID;

void PIDContollerUpdate(float measurement);
void PIDInit();
uint32_t RoundPIDOutput(float output);
float ScaleUpSetpoint(float rpm);
void PIDTIMERHandler();
#endif /* SRC_PID_H_ */

/*
 * PID.c
 *
 *  Created on: May 12, 2023
 *      Author: plasz
 */
#include "PID.h"
volatile PidController PID;

void PIDInit(){

	PID.Kd = 0.1;
	PID.Ki = 2.5;
	PID.Kp = 2.5;
	PID.integral = 0.0;
	PID.last_error = 0;
	PID.output = 0;
	PID.setpoint = 0;
	PID.updated = false;
}

void PIDContollerUpdate(float measurement){



	float error = PID.setpoint - measurement;

    float proportional = PID.Kp * error;

    PID.integral += PID.Ki * error;

    float derivative = PID.Kd * (error - PID.last_error);

    if(PID.setpoint >= 0)
    {
        // Limit the integral term to prevent windup
        if (PID.integral > MAX_PID_OUTPUT) {
        		PID.integral = MAX_PID_OUTPUT;
            }
        else if (PID.integral < MIN_PID_OUTPUT) {
        		PID.integral = MIN_PID_OUTPUT;
            }
        PID.output = proportional + PID.integral + derivative;

        // Limit the output to within the allowable range
        if (PID.output > MAX_PID_OUTPUT) {
        	PID.output = MAX_PID_OUTPUT;
          }
        else if (PID.output < MIN_PID_OUTPUT) {
        	PID.output = MIN_PID_OUTPUT;
          }
        PID.output += PID_OUTPUT_OFFSET;

    }

    if(PID.setpoint < -0)
        {
            // Limit the integral term to prevent windup
            if (PID.integral > MIN_PID_OUTPUT) {
            		PID.integral = MIN_PID_OUTPUT;
                }
            else if (PID.integral < MAX_PID_OUTPUT_NEGATIVE) {
            		PID.integral = MAX_PID_OUTPUT_NEGATIVE;
                }
            PID.output = proportional + PID.integral + derivative;

            // Limit the output to within the allowable range
            if (PID.output > MIN_PID_OUTPUT) {
            	PID.output = MIN_PID_OUTPUT;
              }
            else if (PID.output < MAX_PID_OUTPUT_NEGATIVE) {
            	PID.output = MAX_PID_OUTPUT_NEGATIVE;
              }
            PID.output = PID_OUTPUT_OFFSET + PID.output;
        }

    PID.last_error = error;

}


uint32_t RoundPIDOutput(float output)
{
	float decimal = output- floor(output);

	if(decimal > 0.5)
		decimal = (int) 1;
	else
		decimal = (int) 0;
	return decimal + (int) output;
}

float ScaleUpSetpoint(float rpm){
	return (rpm/MAX_RPM)*MAX_PID_OUTPUT;
}

//void PIDTIMERHandler(){
//    if(1)//car.direction == GOFORWARD)
//    {
//        PIDContollerUpdate(&pid_right, Encoder_right.RPM);
//        PIDContollerUpdate(&pid_left, Encoder_left.RPM);
//        pid_updated = true;}
//}



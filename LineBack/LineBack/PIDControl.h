#ifndef __PIDControl
#define	__PIDControl

#include "config.h"

#define EXECUTE_CYCLE 0.004
#pragma once


typedef struct {
  float Kp;				//Pêßå‰óp
 float Ki;				//Iêßå‰óp
 float Kd;	
}PIDControl;

extern float PIDControl_PIDLineTrace(PIDControl *this_PIDControl,int forwardSpeed);
extern void PIDControl_init(PIDControl *this_PIDControl);


#endif

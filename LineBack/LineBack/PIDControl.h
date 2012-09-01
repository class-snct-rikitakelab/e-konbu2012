#ifndef __PIDControl
#define	__PIDControl

#include "config.h"

#define EXECUTE_CYCLE 0.004
#pragma once


typedef struct {
  float Kp;				//P����p
 float Ki;				//I����p
 float Kd;	
}PIDControl;

extern float PIDControl_PIDLineTrace(PIDControl *this_PIDControl,int forwardSpeed);
extern void PIDControl_init(PIDControl *this_PIDControl);


#endif


#ifndef __Common
#define __Common

#include "ecrobot_interface.h"
#include "balancer.h"

/*
typedef struct {
float forward_val;
float turn_val;
} ControlVals;
*/

typedef struct {
S8 pwmL;
S8 pwmR;
} PWMValues;

typedef struct {
	float Kp;
	float Ki;
	float Kd;
} CtrlParm;

#endif


#ifndef _LIGHT_VAL_H
#define _LIGHT_VAL_H

#include "kernel.h"
#include "kernel_id.h"
#include "ecrobot_interface.h"

//#include "../NXTHardWare/LightSensor.h"
#include "../Common/Factory.h"

typedef struct{
	float targetLightVal;	//�ڕW�P�x�l
}LightVal;

void LightVal_init(LightVal * this_LightVal);
extern U16 LightVal_getLightVal(LightVal * this_LightVal);
extern U16 LightVal_getTargLightVal(LightVal * this_LightVal);
extern void LightVal_setTargLightVal(LightVal * this_LightVal,U16 targLightVal);


#endif

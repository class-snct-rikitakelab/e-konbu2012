#ifndef _LIGHT_VAL_H
#define _LIGHT_VAL_H

#include "kernel.h"
#include "kernel_id.h"
#include "ecrobot_interface.h"

#include "../NXTHardWare/LightSensor.h"

typedef struct{
	float targetLightVal;
}LightVal;

void LightVal_init(LightVal * this_LightVal);
extern U16 LightVal_getLightVal(LightVal * this_LightVal);
extern U16 LightVal_getTargLightVal(LightVal * this_LightVal);
extern void LightVal_setTargLightVal(LightVal * this_LightVal,U16 targLightVal);


#endif

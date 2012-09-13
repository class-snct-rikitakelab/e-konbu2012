#ifndef _LIGHT_VAL_CTRL_H
#define _LIGHT_VAL_CTRL_H

#include "ecrobot_interface.h"
#include "../Factory.h"
#include "../DrivenPart/LightVal.h"

typedef struct{
}LightValCtrl;

void LightValCtrl_init(LightValCtrl *this_LightValCtrl);
extern void LightValCtrl_setTargLightVal(LightValCtrl *this_LightValCtrl, U16 targLightVal);
extern S8 LightValCtrl_doLightValCtrl(LightValCtrl *this_LightValCtrl);

#endif
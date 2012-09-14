#ifndef _P_I_D_LIGHT_VAL_CTRL_H
#define _P_I_D_LIGHT_VAL_CTRL_H

#include "LightValCtrlMethod.h"
#include "ecrobot_interface.h"



typedef struct{
}PIDLightValCtrl;

void PIDLightValCtrl_init(PIDLightValCtrl *this_PIDLightValCtrl);
extern S8 PID_LightValCtrl_calcLightValCtrlVal(PIDLightValCtrl *this_PIDLightValCtrl, U16 targLightVal, U16 lightVal);

#endif

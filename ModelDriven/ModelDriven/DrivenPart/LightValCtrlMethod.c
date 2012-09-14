#include "LightValCtrlMethod.h"

void LightValCtrlMethod_setCtrlParm(PIDLightValCtrl *this_PIDLightValCtrl, CtrlParm parm)
{
	PIDLightValCtrlParm_setLKp(&mPIDLightValCtrlParm,parm.Kp);
	PIDLightValCtrlParm_setLKi(&mPIDLightValCtrlParm,parm.Ki);
	PIDLightValCtrlParm_setLKd(&mPIDLightValCtrlParm,parm.Kd);
}

void LightValCtrlMethod_init(LightValCtrlMethod *this_LightValCtrlMethod)
{
}


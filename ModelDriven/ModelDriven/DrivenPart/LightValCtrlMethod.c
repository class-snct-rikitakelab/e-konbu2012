#include "LightValCtrlMethod.h"
#include "../Common/Factory.h"

extern void LightValCtrlMethod_setCtrlParm(LightValCtrlMethod *this_LightValCtrlMethod,CtrlParm parm)
{
	PIDLightValCtrlParm_setLKp(&mPIDLightValCtrlParm,parm.Kp);
	PIDLightValCtrlParm_setLKi(&mPIDLightValCtrlParm,parm.Ki);
	PIDLightValCtrlParm_setLKd(&mPIDLightValCtrlParm,parm.Kd);
}

void LightValCtrlMethod_init(LightValCtrlMethod *this_LightValCtrlMethod)
{
}


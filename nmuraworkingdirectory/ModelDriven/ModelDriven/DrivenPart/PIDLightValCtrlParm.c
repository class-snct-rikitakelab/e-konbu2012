#include "PIDLightValCtrlParm.h"


float PIDLightValCtrlParm_getLKp(PIDLightValCtrlParm *this_PIDLightValCtrlParm)
{
	return this_PIDLightValCtrlParm->lKp;
}

void PIDLightValCtrlParm_setLKp(PIDLightValCtrlParm *this_PIDLightValCtrlParm, float parm)
{
	this_PIDLightValCtrlParm->lKp = parm;
}

float PIDLightValCtrlParm_getLKi(PIDLightValCtrlParm *this_PIDLightValCtrlParm)
{
	return this_PIDLightValCtrlParm->lKi;
}

void PIDLightValCtrlParm_setLKi(PIDLightValCtrlParm *this_PIDLightValCtrlParm, float parm)
{
	this_PIDLightValCtrlParm->lKi = parm;
}

float PIDLightValCtrlParm_getLKd(PIDLightValCtrlParm *this_PIDLightValCtrlParm)
{
	return this_PIDLightValCtrlParm->lKd;
}

void PIDLightValCtrlParm_setLKd(PIDLightValCtrlParm *this_PIDLightValCtrlParm, float parm)
{
	this_PIDLightValCtrlParm->lKd = parm;
}

void PIDLightValCtrlParm_init(PIDLightValCtrlParm *this_PIDLightValCtrlParm)
{
	this_PIDLightValCtrlParm->lKp = 0.446;
	this_PIDLightValCtrlParm->lKi = 1.2;
	this_PIDLightValCtrlParm->lKd = 0.005;
}


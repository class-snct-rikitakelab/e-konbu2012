#include "PIDTailAngleCtrlParm.h"

float PIDTailAngleCtrlParm_getTKp(PIDTailAngleCtrlParm *this_PIDTailAngleCtrlParm)
{
	return this_PIDTailAngleCtrlParm->tKp;
}

void PIDTailAngleCtrlParm_setTKp(PIDTailAngleCtrlParm *this_PIDTailAngleCtrlParm, float parm)
{
	this_PIDTailAngleCtrlParm->tKp = parm;
}

float PIDTailAngleCtrlParm_getTKi(PIDTailAngleCtrlParm *this_PIDTailAngleCtrlParm)
{
	return this_PIDTailAngleCtrlParm->tKi;
}

void PIDTailAngleCtrlParm_setTKi(PIDTailAngleCtrlParm *this_PIDTailAngleCtrlParm, float parm)
{
	this_PIDTailAngleCtrlParm->tKi = parm;
}

float PIDTailAngleCtrlParm_getTKd(PIDTailAngleCtrlParm *this_PIDTailAngleCtrlParm)
{
	return this_PIDTailAngleCtrlParm->tKd;
}

void PIDTailAngleCtrlParm_setTKd(PIDTailAngleCtrlParm *this_PIDTailAngleCtrlParm, float parm)
{
	this_PIDTailAngleCtrlParm->tKd = parm;
}

void PIDTailAngleCtrlParm_init(PIDTailAngleCtrlParm *this_PIDTailAngleCtrlParm)
{
}
#include "PIDTailAngleCtrlParm.h"

/*
 *	PIDTailAngleCtrlParm.c
 *	K”öŠp“xPID§ŒäƒNƒ‰ƒX
 */

//”ä—áŒW”æ“¾ƒƒ\ƒbƒh
float PIDTailAngleCtrlParm_getTKp(PIDTailAngleCtrlParm *this_PIDTailAngleCtrlParm)
{
	return this_PIDTailAngleCtrlParm->tKp;
}

//”ä—áŒW”İ’èƒƒ\ƒbƒh
void PIDTailAngleCtrlParm_setTKp(PIDTailAngleCtrlParm *this_PIDTailAngleCtrlParm, float parm)
{
	this_PIDTailAngleCtrlParm->tKp = parm;
}

//Ï•ªŒW”æ“¾ƒƒ\ƒbƒh
float PIDTailAngleCtrlParm_getTKi(PIDTailAngleCtrlParm *this_PIDTailAngleCtrlParm)
{
	return this_PIDTailAngleCtrlParm->tKi;
}

//Ï•ªŒW”İ’èƒƒ\ƒbƒh
void PIDTailAngleCtrlParm_setTKi(PIDTailAngleCtrlParm *this_PIDTailAngleCtrlParm, float parm)
{
	this_PIDTailAngleCtrlParm->tKi = parm;
}

//”÷•ªŒW”æ“¾ƒƒ\ƒbƒh
float PIDTailAngleCtrlParm_getTKd(PIDTailAngleCtrlParm *this_PIDTailAngleCtrlParm)
{
	return this_PIDTailAngleCtrlParm->tKd;
}

//Ï•ªŒW”İ’èƒƒ\ƒbƒh
void PIDTailAngleCtrlParm_setTKd(PIDTailAngleCtrlParm *this_PIDTailAngleCtrlParm, float parm)
{
	this_PIDTailAngleCtrlParm->tKd = parm;
}

void PIDTailAngleCtrlParm_init(PIDTailAngleCtrlParm *this_PIDTailAngleCtrlParm)
{
	this_PIDTailAngleCtrlParm->tKp = 8.85;
	this_PIDTailAngleCtrlParm->tKi = 0;
	this_PIDTailAngleCtrlParm->tKd = 0;
}
#include "PIDTailAngleCtrlParm.h"

/*
 *	PIDTailAngleCtrlParm.c
 *	尻尾角度PID制御クラス
 */

//比例係数取得メソッド
float PIDTailAngleCtrlParm_getTKp(PIDTailAngleCtrlParm *this_PIDTailAngleCtrlParm)
{
	return this_PIDTailAngleCtrlParm->tKp;
}

//比例係数設定メソッド
void PIDTailAngleCtrlParm_setTKp(PIDTailAngleCtrlParm *this_PIDTailAngleCtrlParm, float parm)
{
	this_PIDTailAngleCtrlParm->tKp = parm;
}

//積分係数取得メソッド
float PIDTailAngleCtrlParm_getTKi(PIDTailAngleCtrlParm *this_PIDTailAngleCtrlParm)
{
	return this_PIDTailAngleCtrlParm->tKi;
}

//積分係数設定メソッド
void PIDTailAngleCtrlParm_setTKi(PIDTailAngleCtrlParm *this_PIDTailAngleCtrlParm, float parm)
{
	this_PIDTailAngleCtrlParm->tKi = parm;
}

//微分係数取得メソッド
float PIDTailAngleCtrlParm_getTKd(PIDTailAngleCtrlParm *this_PIDTailAngleCtrlParm)
{
	return this_PIDTailAngleCtrlParm->tKd;
}

//積分係数設定メソッド
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
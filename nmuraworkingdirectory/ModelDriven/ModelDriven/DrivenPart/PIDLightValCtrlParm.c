#include "PIDLightValCtrlParm.h"

/*
 *	PIDLightValCtrlParm.c
 *	輝度値PID制御パラメータクラス
 */

//比例係数取得メソッド
float PIDLightValCtrlParm_getLKp(PIDLightValCtrlParm *this_PIDLightValCtrlParm)
{
	return this_PIDLightValCtrlParm->lKp;
}

//比例係数設定メソッド
void PIDLightValCtrlParm_setLKp(PIDLightValCtrlParm *this_PIDLightValCtrlParm, float parm)
{
	this_PIDLightValCtrlParm->lKp = parm;
}

//積分係数取得メソッド
float PIDLightValCtrlParm_getLKi(PIDLightValCtrlParm *this_PIDLightValCtrlParm)
{
	return this_PIDLightValCtrlParm->lKi;
}

//積分係数設定メソッド
void PIDLightValCtrlParm_setLKi(PIDLightValCtrlParm *this_PIDLightValCtrlParm, float parm)
{
	this_PIDLightValCtrlParm->lKi = parm;
}

//微分係数取得メソッド
float PIDLightValCtrlParm_getLKd(PIDLightValCtrlParm *this_PIDLightValCtrlParm)
{
	return this_PIDLightValCtrlParm->lKd;
}

//積分係数設定メソッド
void PIDLightValCtrlParm_setLKd(PIDLightValCtrlParm *this_PIDLightValCtrlParm, float parm)
{
	this_PIDLightValCtrlParm->lKd = parm;
}

//初期化メソッド
void PIDLightValCtrlParm_init(PIDLightValCtrlParm *this_PIDLightValCtrlParm)
{
	this_PIDLightValCtrlParm->lKp = 0.446;
	this_PIDLightValCtrlParm->lKi = 1.2;
	this_PIDLightValCtrlParm->lKd = 0.005;
}


#include "PIDCurvatureCtrlParm.h"

/*
 *	PIDCurvatureCtrlParm.c
 *	曲率半径PID制御パラメータクラス
 */

//比例係数取得メソッド
float PIDCurvatureCtrlParm_getCKp(PIDCurvatureCtrlParm *this_PIDCurvatureCtrlParm)
{
	return this_PIDCurvatureCtrlParm->cKp;
}

//比例係数設定メソッド
void PIDCurvatureCtrlParm_setCKp(PIDCurvatureCtrlParm *this_PIDCurvatureCtrlParm, float parm)
{
	this_PIDCurvatureCtrlParm->cKp = parm;
}

//積分係数取得メソッド
float PIDCurvatureCtrlParm_getCKi(PIDCurvatureCtrlParm *this_PIDCurvatureCtrlParm)
{
	return this_PIDCurvatureCtrlParm->cKi;
}

//積分係数設定メソッド
void PIDCurvatureCtrlParm_setCKi(PIDCurvatureCtrlParm *this_PIDCurvatureCtrlParm, float parm)
{
	this_PIDCurvatureCtrlParm->cKi = parm;
}

//微分係数取得メソッド
float PIDCurvatureCtrlParm_getCKd(PIDCurvatureCtrlParm *this_PIDCurvatureCtrlParm)
{
	return this_PIDCurvatureCtrlParm->cKd;
}

//微分係数設定メソッド
void PIDCurvatureCtrlParm_setCKd(PIDCurvatureCtrlParm *this_PIDCurvatureCtrlParm, float parm)
{
	this_PIDCurvatureCtrlParm->cKd = parm;
}

//初期化メソッド
void PIDCurvatureCtrlParm_init(PIDCurvatureCtrlParm *this_PIDCurvatureCtrlParm)
{
	this_PIDCurvatureCtrlParm->cKp = 0.6;
	this_PIDCurvatureCtrlParm->cKi = 0;
	this_PIDCurvatureCtrlParm->cKd = 0;
}
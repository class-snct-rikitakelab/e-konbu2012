#include "PIDCurvatureCtrl.h"
#include "../Common/Factory.h"

/*
 *	PIDCurvatureCtrl.c
 *	曲率半径PID制御クラス
 */

//曲率半径PID制御メソッド
S32 PIDCurvatureCtrl_calcCurvatureCtrlVal(PIDCurvatureCtrl *this_PIDCurvatureCtrl,float targCurvature,float curvature)
{
	this_PIDCurvatureCtrl->deviation = (float)targCurvature - (float)curvature;

	this_PIDCurvatureCtrl->integratedDeviation = this_PIDCurvatureCtrl->integratedDeviation+(this_PIDCurvatureCtrl->deviation*this_PIDCurvatureCtrl->lastMeasurementTime);
	this_PIDCurvatureCtrl->differentialDeviation = (this_PIDCurvatureCtrl->deviation - this_PIDCurvatureCtrl->bfDeviation)/this_PIDCurvatureCtrl->lastMeasurementTime;
	this_PIDCurvatureCtrl->bfDeviation = this_PIDCurvatureCtrl->deviation;

	return -(PIDCurvatureCtrlParm_getCKp(&mPIDCurvatureCtrlParm) * this_PIDCurvatureCtrl->deviation); /*+ 
		PIDCurvatureCtrlParm_getCKi(&mPIDCurvatureCtrlParm) * this_PIDCurvatureCtrl->integratedDeviation + 
		PIDCurvatureCtrlParm_getCKd(&mPIDCurvatureCtrlParm) * this_PIDCurvatureCtrl->differentialDeviation);*/
}

//制御パラメータ取得メソッド
CtrlParm PIDCurvatureCtrl_getPIDCurvatureCtrlParm(PIDCurvatureCtrl *this_PIDCurvatureCtrl)
{
	CtrlParm getParm;
	getParm.Kp = PIDCurvatureCtrlParm_getCKp(&mPIDCurvatureCtrlParm);
	getParm.Ki = PIDCurvatureCtrlParm_getCKi(&mPIDCurvatureCtrlParm);
	getParm.Kd = PIDCurvatureCtrlParm_getCKd(&mPIDCurvatureCtrlParm);
	return getParm;
}

//制御パラメータ設定メソッド
void PIDCurvatureCtrl_setPIDCurvatureCtrlParm(PIDCurvatureCtrl *this_PIDCurvatureCtrl, CtrlParm parm)
{
	PIDCurvatureCtrlParm_setCKp(&mPIDCurvatureCtrlParm,parm.Kp);
	PIDCurvatureCtrlParm_setCKi(&mPIDCurvatureCtrlParm,parm.Ki);
	PIDCurvatureCtrlParm_setCKd(&mPIDCurvatureCtrlParm,parm.Kd);
}

//初期化メソッド
void PIDCurvatureCtrl_init(PIDCurvatureCtrl *this_PIDCurvatureCtrl)
{
	this_PIDCurvatureCtrl->deviation = 0;
	this_PIDCurvatureCtrl->integratedDeviation = 0;
	this_PIDCurvatureCtrl->differentialDeviation = 0;
	this_PIDCurvatureCtrl->bfDeviation = 0;
	this_PIDCurvatureCtrl->lastMeasurementTime = 0.004;
}


#include "../Common/Factory.h"
#include "PIDTailAngleCtrl.h"


//初期化メソッド
void PIDTailAngleCtrl_init(PIDTailAngleCtrl *this_PIDTailAngleCtrl)
{
	this_PIDTailAngleCtrl->deviation = 0;
	this_PIDTailAngleCtrl->integratedDeviation = 0;
	this_PIDTailAngleCtrl->differentialDeviation = 0;
	this_PIDTailAngleCtrl->bfDeviation = 0;
	this_PIDTailAngleCtrl->lastMeasurementTime = 0.004;
}

//制御パラメータ設定メソッド
void PIDTailAngleCtrl_setPIDTailAngleCtrlParm(PIDTailAngleCtrl *this_PIDTailAngleCtrl, CtrlParm parm)
{
	PIDTailAngleCtrlParm_setTKp(&mPIDTailAngleCtrlParm,parm.Kp);
	PIDTailAngleCtrlParm_setTKp(&mPIDTailAngleCtrlParm,parm.Ki);
	PIDTailAngleCtrlParm_setTKp(&mPIDTailAngleCtrlParm,parm.Kd);
}

//制御パラメータ取得メソッド
CtrlParm PIDTailAngleCtrl_getPIDTailAngleCtrlParm(PIDTailAngleCtrl *this_PIDTailAngleCtrl)
{
	CtrlParm getParm;
	getParm.Kp = PIDTailAngleCtrlParm_getTKp(&mPIDTailAngleCtrlParm);
	getParm.Ki = PIDTailAngleCtrlParm_getTKi(&mPIDTailAngleCtrlParm);
	getParm.Kd = PIDTailAngleCtrlParm_getTKd(&mPIDTailAngleCtrlParm);
	return getParm;
}

//尻尾角度PID制御メソッド
S8 PIDTailAngleCtrl_calcTailAngleCtrlVal(PIDTailAngleCtrl *this_PIDTailAngleCtrl, int targTailAngle, int tailAngle)
{
	/*
	 *	しっぽ目標値制御（他に入れられないか検討中）
	 */
	
	static int counter = 0;
	static int resultAngle = 0;

	counter++;
	
	if(targTailAngle == 0)
		resultAngle = 0;
		
	if(counter >= TAIL_ANGLE_COUNT_MAX)
	{
		if(targTailAngle - resultAngle >= 0){
			resultAngle++;

			if(resultAngle > targTailAngle)
				resultAngle = targTailAngle;
		}
		else{
			resultAngle--;

			if(resultAngle < targTailAngle)
				resultAngle = targTailAngle;
		}
		counter = 0;
	}
	

	/*
	 *	しっぽ制御量算出
	 */

	S32 t_speed;

	this_PIDTailAngleCtrl->deviation = (float)targTailAngle - (float)tailAngle;
	
	this_PIDTailAngleCtrl->integratedDeviation = this_PIDTailAngleCtrl->integratedDeviation+(this_PIDTailAngleCtrl->deviation*this_PIDTailAngleCtrl->lastMeasurementTime);
	this_PIDTailAngleCtrl->differentialDeviation = (this_PIDTailAngleCtrl->deviation - this_PIDTailAngleCtrl->bfDeviation)/this_PIDTailAngleCtrl->lastMeasurementTime;
	this_PIDTailAngleCtrl->bfDeviation = this_PIDTailAngleCtrl->deviation;

	t_speed = PIDTailAngleCtrl_getPIDTailAngleCtrlParm(&mPIDTailAngleCtrl).Kp * this_PIDTailAngleCtrl->deviation; + 
		PIDTailAngleCtrlParm_getTKi(&mPIDTailAngleCtrlParm) * this_PIDTailAngleCtrl->integratedDeviation + 
		PIDTailAngleCtrlParm_getTKd(&mPIDTailAngleCtrlParm) * this_PIDTailAngleCtrl->differentialDeviation;
		
	if (t_speed < -100)
		t_speed = -100;
	else if (t_speed > 100)
		t_speed = 100;
		
	return (S8)t_speed;
}

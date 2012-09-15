#include "PIDLightValCtrl.h"
#include "../Common/Factory.h"

/*
 *	PIDLightValCtrl.c
 *	�P�x�lPID����N���X
 */

//���������\�b�h
void PIDLightValCtrl_init(PIDLightValCtrl *this_PIDLightValCtrl)
{
	this_PIDLightValCtrl->deviation = 0;
	this_PIDLightValCtrl->integratedDeviation = 0;
	this_PIDLightValCtrl->differentialDeviation = 0;
	this_PIDLightValCtrl->bfDeviation = 0;
	this_PIDLightValCtrl->lastMeasurementTime = 0.004;
}

//PID�p�����[�^�ݒ胁�\�b�h
void LightValCtrlMethod_setCtrlParm(PIDLightValCtrl *this_PIDLightValCtrl, CtrlParm parm)
{
	PIDLightValCtrlParm_setLKp(&mPIDLightValCtrlParm,parm.Kp);
	PIDLightValCtrlParm_setLKi(&mPIDLightValCtrlParm,parm.Ki);
	PIDLightValCtrlParm_setLKd(&mPIDLightValCtrlParm,parm.Kd);
}

//PID����v�Z���\�b�h
S8 PID_LightValCtrl_calcLightValCtrlVal(PIDLightValCtrl *this_PIDLightValCtrl, U16 targLightVal, U16 lightVal)
{
	this_PIDLightValCtrl->deviation = (float)targLightVal - (float)lightVal;

	this_PIDLightValCtrl->integratedDeviation = this_PIDLightValCtrl->integratedDeviation+(this_PIDLightValCtrl->deviation*this_PIDLightValCtrl->lastMeasurementTime);
	this_PIDLightValCtrl->differentialDeviation = (this_PIDLightValCtrl->deviation - this_PIDLightValCtrl->bfDeviation)/this_PIDLightValCtrl->lastMeasurementTime;
	this_PIDLightValCtrl->bfDeviation = this_PIDLightValCtrl->deviation;
	
	return (PIDLightValCtrlParm_getLKp(&mPIDLightValCtrlParm) * this_PIDLightValCtrl->deviation + 
		PIDLightValCtrlParm_getLKi(&mPIDLightValCtrlParm) * this_PIDLightValCtrl->integratedDeviation + 
		PIDLightValCtrlParm_getLKd(&mPIDLightValCtrlParm) * this_PIDLightValCtrl->differentialDeviation);
}


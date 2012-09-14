#ifndef _P_I_D_LIGHT_VAL_CTRL_H
#define _P_I_D_LIGHT_VAL_CTRL_H

#include "LightValCtrlMethod.h"
#include "ecrobot_interface.h"
#include "PIDLightValCtrlParm.h"
#include "../Common/Factory.h"

typedef struct{
	float deviation;			//�΍�
	float integratedDeviation;	//�ϕ��΍�
	float differentialDeviation;//�����΍�
	float bfDeviation;			//�O��l
	float lastMeasurementTime;	//�O�񂩂�̌v������
}PIDLightValCtrl;

extern S8 PID_LightValCtrl_calcLightValCtrlVal(PIDLightValCtrl *this_PIDLightValCtrl, U16 targLightVal, U16 lightVal);
void PIDLightValCtrl_init(PIDLightValCtrl *this_PIDLightValCtrl);

#endif

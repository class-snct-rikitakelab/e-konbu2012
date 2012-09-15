#ifndef _P_I_D_LIGHT_VAL_CTRL_H
#define _P_I_D_LIGHT_VAL_CTRL_H

#include "ecrobot_interface.h"
#include "PIDLightValCtrlParm.h"

typedef struct{
	float deviation;			//偏差
	float integratedDeviation;	//積分偏差
	float differentialDeviation;//微分偏差
	float bfDeviation;			//前回値
	float lastMeasurementTime;	//前回からの計測時間
}PIDLightValCtrl;

extern S32 PID_LightValCtrl_calcLightValCtrlVal(PIDLightValCtrl *this_PIDLightValCtrl, U16 targLightVal, U16 lightVal);
void PIDLightValCtrl_init(PIDLightValCtrl *this_PIDLightValCtrl);

#endif

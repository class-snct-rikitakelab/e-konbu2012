#ifndef _P_I_D_TAIL_ANGLE_CTRL_H
#define _P_I_D_TAIL_ANGLE_CTRL_H

/**
 * 尻尾角度PID制御方式
 * 
 */

#include "../DrivenPart/PIDTailAngleCtrlParm.h"
#include "../Common/Common.h"

typedef struct{
	float deviation;				//比例係数
	float integratedDeviation;		//積分係数
	float differentialDeviation;	//微分係数
	float bfDeviation;				//前回の比例係数
	float lastMeasurementTime;		//前回の計測時間
	PIDTailAngleCtrlParm Parm;		//集約用（グレーゾーン）
}PIDTailAngleCtrl;

#define TAIL_ANGLE_COUNT_MAX 3		//目標値上昇用カウンタ

#define SPEED_LIMIT 100

extern S8 PIDTailAngleCtrl_calcTailAngleCtrlVal(PIDTailAngleCtrl *thisPIDTailAngleCtrl, int targTailAngle, int tailAngle);
extern void PIDTailAngleCtrl_init(PIDTailAngleCtrl *this_PIDTailAngleCtrl);
extern void PIDTailAngleCtrl_setPIDTailAngleCtrlParm(PIDTailAngleCtrl *this_PIDTailAngleCtrl, CtrlParm parm);
extern CtrlParm PIDTailAngleCtrl_getPIDTailAngleCtrlParm(PIDTailAngleCtrl *this_PIDTailAngleCtrl);

#endif

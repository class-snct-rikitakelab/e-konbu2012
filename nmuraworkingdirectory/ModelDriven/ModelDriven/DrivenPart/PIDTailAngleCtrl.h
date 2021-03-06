#ifndef _P_I_D_TAIL_ANGLE_CTRL_H
#define _P_I_D_TAIL_ANGLE_CTRL_H

/**
 * KöpxPID§äû®
 * 
 */

#include "../DrivenPart/PIDTailAngleCtrlParm.h"
#include "../Common/Common.h"

typedef struct{
	float deviation;				//äáW
	float integratedDeviation;		//ÏªW
	float differentialDeviation;	//÷ªW
	float bfDeviation;				//OñÌäáW
	float lastMeasurementTime;		//OñÌvªÔ
	PIDTailAngleCtrlParm Parm;		//WñpiO[][j
}PIDTailAngleCtrl;

#define TAIL_ANGLE_COUNT_MAX 3		//ÚWlã¸pJE^

#define SPEED_LIMIT 100

extern S8 PIDTailAngleCtrl_calcTailAngleCtrlVal(PIDTailAngleCtrl *thisPIDTailAngleCtrl, int targTailAngle, int tailAngle);
extern void PIDTailAngleCtrl_init(PIDTailAngleCtrl *this_PIDTailAngleCtrl);
extern void PIDTailAngleCtrl_setPIDTailAngleCtrlParm(PIDTailAngleCtrl *this_PIDTailAngleCtrl, CtrlParm parm);
extern CtrlParm PIDTailAngleCtrl_getPIDTailAngleCtrlParm(PIDTailAngleCtrl *this_PIDTailAngleCtrl);

#endif

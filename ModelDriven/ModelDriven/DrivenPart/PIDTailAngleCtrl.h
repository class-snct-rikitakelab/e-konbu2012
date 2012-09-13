#ifndef ‹ì“®•”_P_I_D_TAIL_ANGLE_CTRL_H
#define ‹ì“®•”_P_I_D_TAIL_ANGLE_CTRL_H



/**
 * K”öŠp“xPID§Œä•û®
 * 
 */

#include "../DrivenPart/PIDTailAngleCtrlParm.h"
#include "../Common.h"
#include "../Common/Factory.h"

typedef struct{
	float deviation;
	float integratedDeviation;
	int lastMeasurementTime;
	PIDTailAngleCtrlParm Parm;
}PIDTailAngleCtrl;

#define TAIL_ANGLE_COUNT_MAX 10

extern S8 PIDTailAngleCtrl_calcTailAngleCtrlVal(PIDTailAngleCtrl *thisPIDTailAngleCtrl, U16 targTailAngle, U16 tailAngle);
extern void PIDTailAngleCtrl_init(PIDTailAngleCtrl *this_PIDTailAngleCtrl);
extern void PIDTailAngleCtrl_setPIDTailAngleCtrlParm(PIDTailAngleCtrl *this_PIDTailAngleCtrl, CtrlParm parm);
extern CtrlParm PIDTailAngleCtrl_getPIDTailAngleCtrlParm(PIDTailAngleCtrl *this_PIDTailAngleCtrl);

#endif

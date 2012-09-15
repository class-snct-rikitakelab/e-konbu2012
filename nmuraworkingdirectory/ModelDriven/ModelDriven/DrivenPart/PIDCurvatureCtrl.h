#ifndef _P_I_D_CURVATURE_CTRL_H
#define _P_I_D_CURVATURE_CTRL_H

#include "../DrivenPart/PIDCurvatureCtrlParm.h"
#include "ecrobot_interface.h"
#include "../Common/Common.h"

typedef struct{
	float deviation;			//”ä—áŒW”
	float integratedDeviation;	//Ï•ªŒW”
	float differentialDeviation;//”÷•ªŒW”
	float bfDeviation;			//bf
	float lastMeasurementTime;	//‘O‰ñ‚©‚ç‚ÌŒv‘ªŠÔ
	PIDCurvatureCtrlParm pIDCurvatureCtrlParm;
}PIDCurvatureCtrl;

extern S32 PIDCurvatureCtrl_calcCurvatureCtrlVal(PIDCurvatureCtrl *this_PIDCurvatureCtrl,float targCurvature,float curvature);

extern CtrlParm PIDCurvatureCtrl_getPIDCurvatureCtrlParm(PIDCurvatureCtrl *this_PIDCurvatureCtrl);

extern void PIDCurvatureCtrl_setPIDCurvatureCtrlParm(PIDCurvatureCtrl *this_PIDCurvatureCtrl, CtrlParm parm);

extern void PIDCurvatureCtrl_init(PIDCurvatureCtrl *this_PIDCurvatureCtrl);

#endif

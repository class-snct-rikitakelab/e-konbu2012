
#include "TargetDrivenParm.h"

void TargetDrivenParm_init(TargetDrivenParm *this_TargetDrivenParm,
	S8 targForwardVal,float curvature,RUN_MODE runMode,int tailAngle,U16 targLightVal,TARG_CTRL_METHOD targCtrlMethod,U16 whiteVal,U16 blackVal,U16 volt,U16 gyroOffset)
{

	this_TargetDrivenParm->blackVal =  this_TargetDrivenParm->blackVal + blackVal;
	this_TargetDrivenParm->whiteVal =  this_TargetDrivenParm->blackVal + whiteVal;

	this_TargetDrivenParm->curvature =  curvature;
	this_TargetDrivenParm->gyroOffset =  gyroOffset;
	this_TargetDrivenParm->runMode =  runMode;
	this_TargetDrivenParm->tailAngle = tailAngle;
	this_TargetDrivenParm->targCtrlMethod =  targCtrlMethod;
	this_TargetDrivenParm->targForwardVal =  targForwardVal;
	this_TargetDrivenParm->targLightVal =  targLightVal;
	this_TargetDrivenParm->volt =  volt;
	
}

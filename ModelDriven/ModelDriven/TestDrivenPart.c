#include "TestDrivenPart.h"

#include "Common/Factory.h"


void setTargVals(TargetDrivenParm parm2){
	//parm.targForwardVal =0;
	//parm.runMode=BALANCING;
		TargetDrivenParm parm;
	parm.blackVal = 560;
	parm.curvature = 0.0;
	parm.gyroOffset = 0;
	parm.runMode = BALANCING;
	parm.tailAngle = 95;
	parm.targCtrlMethod = LIGHT_PID;
	parm.targForwardVal = 20;
	parm.targLightVal = 600;
	parm.volt = 7800;
	parm.whiteVal = 620;

	RobotDrivenDirect_setDriveParm(&mRobotDrivenDirect,parm);
}

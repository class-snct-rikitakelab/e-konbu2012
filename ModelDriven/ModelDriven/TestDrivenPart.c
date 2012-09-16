#include "TestDrivenPart.h"

#include "Common/Factory.h"


void setTargVals(TargetDrivenParm parm){
	//parm.targForwardVal =0;
	//parm.runMode=BALANCING;
	RobotDrivenDirect_seDriveParm(&mRobotDrivenDirect,parm);
}

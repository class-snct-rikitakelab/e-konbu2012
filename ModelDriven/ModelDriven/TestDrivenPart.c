#include "TestDrivenPart.h"

#include "Common\Factory.h"






void setTargVals(TargetDrivenParm parm){
	parm.targForwardVal =20;
	parm.runMode=BALANCING;
	RobotDrivenDirect_seDriveParm(&mRobotDrivenDirect,parm);

}
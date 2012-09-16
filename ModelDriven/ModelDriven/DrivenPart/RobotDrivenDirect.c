

#include "RobotDrivenDirect.h"

#include "../Common/Factory.h"


void RobotDrivenDirect_init(RobotDrivenDirect *this_RobotDrivenDirect){
	RobotDrivenDirect_seDriveParm(this_RobotDrivenDirect,idleSector.targetDrvenParm);
	//none
}

	/**
	 * 駆動指示
	 */
void RobotDrivenDirect_directDrive(RobotDrivenDirect *this_RobotDrivenDirect){
	WheelMotorCtrl_doMotorCtrl(&mWheelMotorCtrl);
}

	/**
	 * 目標駆動パラメータを設定する
	 */
void RobotDrivenDirect_seDriveParm(RobotDrivenDirect *this_RobotDrivenDirect, TargetDrivenParm parm){
	WheelMotorCtrl_setMotionTargVal(&mWheelMotorCtrl,parm);
	TailAngleCtrl_setTargTailAngle(&mTailAngleCtrl,parm.tailAngle);
}

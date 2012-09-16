#include "RobotDrivenDirect.h"
#include "../Common/Factory.h"

/*
 *	RobotDrivenDirect.c
 *	駆動指示器クラス
 */

void RobotDrivenDirect_init(RobotDrivenDirect *this_RobotDrivenDirect){
	//none
}

	/**
	 * 駆動指示
	 */
void RobotDrivenDirect_directDrive(RobotDrivenDirect *this_RobotDrivenDirect){
	WheelMotorCtrl_doMotorCtrl(&mWheelMotorCtrl);
	TailAngleCtrl_doTailCtrl(&mTailAngleCtrl);
}

	/**
	 * 目標駆動パラメータを設定する
	 */
void RobotDrivenDirect_seDriveParm(RobotDrivenDirect *this_RobotDrivenDirect, TargetDrivenParm parm){
	WheelMotorCtrl_setMotionTargVal(&mWheelMotorCtrl,parm);
	TailAngleCtrl_setTargTailAngle(&mTailAngleCtrl,parm.tailAngle);
}

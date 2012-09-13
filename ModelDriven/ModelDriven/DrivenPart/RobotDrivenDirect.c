

#include "RobotDrivenDirect.h"

#include "../Common/Factory.h"


void RobotDrivenDirect_init(RobotDrivenDirect *this_RobotDrivenDirect){
	//none
}

	/**
	 * �쓮�w��
	 */
void RobotDrivenDirect_directDrive(RobotDrivenDirect *this_RobotDrivenDirect){
	WheelMotorCtrl_doMotorCtrl(&mWheelMotorCtrl);
}

	/**
	 * �ڕW�쓮�p�����[�^��ݒ肷��
	 */
void RobotDrivenDirect_seDriveParm(RobotDrivenDirect *this_RobotDrivenDirect, TargetDrivenParm parm){
	WheelMotorCtrl_setMotionTargVal(&mWheelMotorCtrl,parm);
	TailAngleCtrl_setTargTailAngle(&mTailAngleCtrl,parm.tailAngle);
}

#ifndef __RobotDrivenDirect
#define __RobotDrivenDirect

#include "../StrategyPart/TargetDrivenParm.h"
#include "../DrivenPart/TailAngleCtrl.h"
#include "../DrivenPart/WheelMotorCtrl.h"
#include "../DirectorPart/Running.h"
#include "../Common/Factory.h"
/**
 * 車体駆動指示器
 */
typedef struct {
	TargetDrivenParm drivenParm;
}RobotDrivenDirect

	void RobotDrivenDirect_init(RobotDrivenDirect *this_RobotDrivenDirect);

	/**
	 * 駆動指示
	 */
	void RobotDrivenDirect_directDrive(RobotDrivenDirect *this_RobotDrivenDirect);

	/**
	 * 目標駆動パラメータを設定する
	 */
	void RobotDrivenDirect_seDriveParm(RobotDrivenDirect *this_RobotDrivenDirect,TargetDrivenParm parm);

#endif
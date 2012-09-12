
#include <string>
#include <vector>
#include <list>
#include <iostream>
#include <assert.h>

#include "StrategyPart/TargetDrivenParm.h"
#include "DrivenPart/TailAngleCtrl.h"
#include "DrivenPart/WheelMotorCtrl.h"
#include "DirectorPart/Running.h"

namespace DrivenPart
{
/**
 * 車体駆動指示器
 */
class RobotDrivenDirect
{
private:
	StrategyPart::TargetDrivenParm drivenParm;

	DirectorPart::Running running;
	TailAngleCtrl tailAngleCtrl;
	TailAngleCtrl 尻尾角度制御;
	WheelMotorCtrl 車輪モータ制御量算出;

public:
	/**
	 * 駆動指示
	 */
	void directDrive(RobotDrivenDirect *this_RobotDrivenDirect);

	/**
	 * 目標駆動パラメータを設定する
	 */
	void RobotDrivenDirect_seDriveParm(RobotDrivenDirect *this_RobotDrivenDirect, StrategyPart::TargetDrivenParm parm);

};

}  // namespace DrivenPart

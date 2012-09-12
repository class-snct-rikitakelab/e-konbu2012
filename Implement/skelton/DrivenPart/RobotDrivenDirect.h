
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
 * �ԑ̋쓮�w����
 */
class RobotDrivenDirect
{
private:
	StrategyPart::TargetDrivenParm drivenParm;

	DirectorPart::Running running;
	TailAngleCtrl tailAngleCtrl;
	TailAngleCtrl �K���p�x����;
	WheelMotorCtrl �ԗփ��[�^����ʎZ�o;

public:
	/**
	 * �쓮�w��
	 */
	void directDrive(RobotDrivenDirect *this_RobotDrivenDirect);

	/**
	 * �ڕW�쓮�p�����[�^��ݒ肷��
	 */
	void RobotDrivenDirect_seDriveParm(RobotDrivenDirect *this_RobotDrivenDirect, StrategyPart::TargetDrivenParm parm);

};

}  // namespace DrivenPart


#include "../StrategyPart/TargetDrivenParm.h"
#include "../DrivenPart/TailAngleCtrl.h"
#include "../DrivenPart/WheelMotorCtrl.h"
#include "../DirectorPart/Running.h"
#include "../Factory.h"
/**
 * �ԑ̋쓮�w����
 */
typedef struct {
	TargetDrivenParm drivenParm;
}RobotDrivenDirect

	void RobotDrivenDirect_init(RobotDrivenDirect *this_RobotDrivenDirect);

	/**
	 * �쓮�w��
	 */
	void RobotDrivenDirect_directDrive(RobotDrivenDirect *this_RobotDrivenDirect);

	/**
	 * �ڕW�쓮�p�����[�^��ݒ肷��
	 */
	void RobotDrivenDirect_seDriveParm(RobotDrivenDirect *this_RobotDrivenDirect,TargetDrivenParm parm);

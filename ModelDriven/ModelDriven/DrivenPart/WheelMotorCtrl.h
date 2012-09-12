
#include <string>
#include <vector>
#include <list>
#include <iostream>
#include <assert.h>

#include "StrategyPart/TargetDrivenParm.h"
#include "DrivenPart/ForwardValRevise.h"
#include "DrivenPart/LightValCtrl.h"
#include "DrivenPart/CurvatureCtrl.h"
#include "DrivenPart/WheelMotor.h"
#include "DrivenPart/S8.h"
#include "NXTHardWare/GyroSensor.h"

namespace DrivenPart
{
/**
 * �ԗփ��[�^����
 */
class WheelMotorCtrl
{
private:
	/**
	 * ���s���[�h
	 */
	int runMode;

	/**
	 * �E���[�^�����
	 * �EPWM�l
	 */
	int rightMotorCtrlVal;

	/**
	 * �����[�^�����
	 * ��PWM�l
	 */
	int leftMotorCtrlVal;

	/**
	 * �ڕW�������
	 */
	int targCtrlMethod;

	ForwardValRevise �O�i�ʎZ�o;
	LightValCtrl ����ʎZ�o;
	CurvatureCtrl ����ʎZ�o;
	WheelMotor �E�ԗփT�[�{���[�^;
	WheelMotor ���ԗփT�[�{���[�^;
	NXTHardWare::GyroSensor gyroSensor;

public:
	/**
	 * ���[�^�𐧌䂷��
	 */
	void WheelMotorCtrl_doMotorCtrl(WheelMotorCtrl *this_WheelMotorCtrl);

	/**
	 * �|������p���[�^����ʂ��Z�o����
	 */
	void WheelMotorCtrl_calcBalancingMotorCtrlVal(WheelMotorCtrl *this_WheelMotorCtrl, S8 rightMotorCtrlVal, S8 leftMotorCtrlVal);

	/**
	 * �K�����s�p���[�^����ʂ��Z�o����
	 */
	void WheelMotorCtrl_calcTailRunningMotorCtrlVal(WheelMotorCtrl *this_WheelMotorCtrl, S8 rightMotorCtrlVal, S8 leftMotorCtrlVal);

	/**
	 * ����ڕW�l��ݒ肷��
	 */
	void WheelMotorCtrl_setMotionTargVal(WheelMotorCtrl *this_WheelMotorCtrl, StrategyPart::TargetDrivenParm parm);

};

}  // namespace DrivenPart


#include <string>
#include <vector>
#include <list>
#include <iostream>
#include <assert.h>

#include "DrivenPart/S8.h"
#include "DrivenPart/WheelMotorCtrl.h"
#include "DetectionPart/U16.h"

namespace StrategyPart
{
/**
 * �ڕW�쓮�p�����[�^
 */
class TargetDrivenParm
{
private:
	/**
	 * �ڕW�O�i���x
	 */
	DrivenPart::S8 targForwardVal;

	/**
	 * �ȗ����a
	 */
	float Curvature;

	/**
	 * ���s���[�h
	 */
	int runMode;

	/**
	 * �K���p�x
	 */
	int tailAngle;

	/**
	 * �g�p�����
	 */
	int useController;

	/**
	 * �ڕW�P�x�l
	 */
	DetectionPart::U16 TargLightVal;

	/**
	 * �ڕW�������
	 */
	int targCtrlMethod;

	int whiteVal;

	/**
	 * ���F�P�x�l
	 */
	int blackVal;

	/**
	 * �d���l
	 */
	int volt;

	/**
	 * �W���C���I�t�Z�b�g
	 */
	int gyroOffset;

	DrivenPart::WheelMotorCtrl wheelMotorCtrl;
};

}  // namespace StrategyPart

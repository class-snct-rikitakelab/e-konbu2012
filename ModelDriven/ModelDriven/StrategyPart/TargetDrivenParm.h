#ifndef __TargetDriveParm
#define __TargetDriveParm


#include "../DrivenPart/WheelMotorCtrl.h"
#include "../Common/RUN_MODE.h"
#include "../Common/TARG_CTRL_METHOD.h"

/**
 * �ڕW�쓮�p�����[�^
 */
typedef struct 
{

	/**
	 * �ڕW�O�i���x
	 */
	S8 targForwardVal;

	/**
	 * �ȗ����a
	 */
	float curvature;

	/**
	 * ���s���[�h
	 */
	RUN_MODE runMode;

	/**
	 * �K���p�x
	 */
	int tailAngle;

	
	/**
	 * �ڕW�P�x�l
	 */
	U16 targLightVal;

	/**
	 * �ڕW�������
	 */
	TARG_CTRL_METHOD targCtrlMethod;

	/**
	 * ���F�P�x�l
	 */
	
	S8 whiteVal;

	/**
	 * ���F�P�x�l
	 */
	S8 blackVal;

	/**
	 * �d���l
	 */
	U16 volt;

	/**
	 * �W���C���I�t�Z�b�g
	 */
	U16 gyroOffset;

}TargetDrivenParm;

#endif
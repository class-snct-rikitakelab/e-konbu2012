#ifndef __TargetDriveParm
#define __TargetDriveParm


#include "../Common/RUN_MODE.h"
#include "../Common/TARG_CTRL_METHOD.h"
#include "ecrobot_interface.h"

/**
 * �ڕW�쓮�p�����[�^
 */
typedef struct {

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
	
	U16 whiteVal;

	/**
	 * ���F�P�x�l
	 */
	U16 blackVal;

	/**
	 * �d���l
	 */
	U16 volt;

	/**
	 * �W���C���I�t�Z�b�g
	 */
	U16 gyroOffset;

}TargetDrivenParm;
void TargetDrivenParm_init(TargetDrivenParm *this_TargetDrivenParm,
	S8 targForwardVal,float curvature,RUN_MODE runMode,int tailAngle,U16 targLightVal,TARG_CTRL_METHOD targCtrlMethod,U16 whiteVal,U16 blackVal,U16 volt,U16 gyroOffset);


#endif


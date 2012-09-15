#ifndef __TargetDriveParm
#define __TargetDriveParm


#include "../Common/RUN_MODE.h"
#include "../Common/TARG_CTRL_METHOD.h"
#include "ecrobot_interface.h"

/**
 * 目標駆動パラメータ
 */
typedef struct {

	/**
	 * 目標前進速度
	 */
	S8 targForwardVal;

	/**
	 * 曲率半径
	 */
	float curvature;

	/**
	 * 走行モード
	 */
	RUN_MODE runMode;

	/**
	 * 尻尾角度
	 */
	int tailAngle;

	
	/**
	 * 目標輝度値
	 */
	U16 targLightVal;

	/**
	 * 目標制御方式
	 */
	TARG_CTRL_METHOD targCtrlMethod;

	/**
	 * 白色輝度値
	 */
	
	U16 whiteVal;

	/**
	 * 黒色輝度値
	 */
	U16 blackVal;

	/**
	 * 電圧値
	 */
	U16 volt;

	/**
	 * ジャイロオフセット
	 */
	U16 gyroOffset;

}TargetDrivenParm;

#endif


#ifndef __WheelMotorCtrl
#define __WheelMotorCtrl


#include "balancer.h"
#include "../StrategyPart/TargetDrivenParm.h"
#include "../DrivenPart/ForwardValRevise.h"
#include "../DrivenPart/LightValCtrl.h"
#include "../DrivenPart/CurvatureCtrl.h"
#include "../DrivenPart/WheelMotor.h"
#include "../DrivenPart/S8.h"
#include "../NXTHardWare/GyroSensor.h"
#include "RUN_MODE.h"
#include "TARG_CTRL_METHOD.h"

/**
 * 車輪モータ制御
 */
typedef struct {
	/**
	 * 走行モード
	 */
	RUN_MODE runMode;

	/**
	 * 右モータ制御量
	 * 右PWM値
	 */
	S8 rightMotorCtrlVal;

	/**
	 * 左モータ制御量
	 * 左PWM値
	 */
	S8 leftMotorCtrlVal;

	/**
	 * 目標制御方式
	 */
	
	TARG_CTRL_METHOD targCtrlMethod;

} WheelMotorCtrl;


	void WheelMotorCtrl_init(WheelMotorCtrl *this_WheelMotorCtrl);

	/**
	 * モータを制御する
	 */
	void WheelMotorCtrl_doMotorCtrl(WheelMotorCtrl *this_WheelMotorCtrl);

	/**
	 * 倒立制御用モータ制御量を算出する
	 */
	void WheelMotorCtrl_calcBalancingMotorCtrlVal(WheelMotorCtrl *this_WheelMotorCtrl, S8 rightMotorCtrlVal, S8 leftMotorCtrlVal);

	/**
	 * 尻尾走行用モータ制御量を算出する
	 */
	void WheelMotorCtrl_calcTailRunningMotorCtrlVal(WheelMotorCtrl *this_WheelMotorCtrl, S8 rightMotorCtrlVal, S8 leftMotorCtrlVal);

	/**
	 * 動作目標値を設定する
	 */
	void WheelMotorCtrl_setMotionTargVal(WheelMotorCtrl *this_WheelMotorCtrl, StrategyPart::TargetDrivenParm parm);


#endif

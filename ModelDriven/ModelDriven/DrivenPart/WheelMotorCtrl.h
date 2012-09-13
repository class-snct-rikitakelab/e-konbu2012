#ifndef __WheelMotorCtrl
#define __WheelMotorCtrl


#include "balancer.h"
#include "../StrategyPart/TargetDrivenParm.h"
#include "../DrivenPart/ForwardValRevise.h"
#include "../DrivenPart/LightValCtrl.h"
#include "../DrivenPart/CurvatureCtrl.h"
#include "../DrivenPart/WheelMotor.h"
#include "../NXTHardWare/GyroSensor.h"
#include "../Common/RUN_MODE.h"
#include "../Common/TARG_CTRL_METHOD.h"

/**
 * 車輪モータ制御
 */
typedef struct {
	/**
	ジャイロオフセット
	*/
	U16 gyroOffset;

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
	void WheelMotorCtrl_calcBalancingMotorCtrlVal(WheelMotorCtrl *this_WheelMotorCtrl,int forward,int turn,int gyroOffset);
	/**
	 * 尻尾走行用モータ制御量を算出する
	 */
	void WheelMotorCtrl_calcTailRunningMotorCtrlVal(WheelMotorCtrl *this_WheelMotorCtrl,int forward,int turn);
	
	/**
	 * 動作目標値を設定する
	 */
	void WheelMotorCtrl_setMotionTargVal(WheelMotorCtrl *this_WheelMotorCtrl,TargetDrivenParm parm);
	
	//モータ制御量算出
	S8 WheelMotorCtrl_calTurnVal(WheelMotorCtrl *this_WheelMotorCtrl);//turn値算出　内部で尻尾か倒立か状態を判断して算出


#endif




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
 * 車輪モータ制御
 */
class WheelMotorCtrl
{
private:
	/**
	 * 走行モード
	 */
	int runMode;

	/**
	 * 右モータ制御量
	 * 右PWM値
	 */
	int rightMotorCtrlVal;

	/**
	 * 左モータ制御量
	 * 左PWM値
	 */
	int leftMotorCtrlVal;

	/**
	 * 目標制御方式
	 */
	int targCtrlMethod;

	ForwardValRevise 前進量算出;
	LightValCtrl 旋回量算出;
	CurvatureCtrl 旋回量算出;
	WheelMotor 右車輪サーボモータ;
	WheelMotor 左車輪サーボモータ;
	NXTHardWare::GyroSensor gyroSensor;

public:
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

};

}  // namespace DrivenPart

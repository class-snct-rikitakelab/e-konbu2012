
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
 * 目標駆動パラメータ
 */
class TargetDrivenParm
{
private:
	/**
	 * 目標前進速度
	 */
	DrivenPart::S8 targForwardVal;

	/**
	 * 曲率半径
	 */
	float Curvature;

	/**
	 * 走行モード
	 */
	int runMode;

	/**
	 * 尻尾角度
	 */
	int tailAngle;

	/**
	 * 使用制御器
	 */
	int useController;

	/**
	 * 目標輝度値
	 */
	DetectionPart::U16 TargLightVal;

	/**
	 * 目標制御方式
	 */
	int targCtrlMethod;

	int whiteVal;

	/**
	 * 黒色輝度値
	 */
	int blackVal;

	/**
	 * 電圧値
	 */
	int volt;

	/**
	 * ジャイロオフセット
	 */
	int gyroOffset;

	DrivenPart::WheelMotorCtrl wheelMotorCtrl;
};

}  // namespace StrategyPart

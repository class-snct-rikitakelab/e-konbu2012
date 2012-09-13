#ifndef 駆動部_P_I_D_CURVATURE_CTRL_H
#define 駆動部_P_I_D_CURVATURE_CTRL_H

#include <string>
#include <vector>
#include <list>
#include <iostream>
#include <assert.h>

#include "駆動部/PIDCurvatureCtrlParm.h"
#include "駆動部/PIDCurvatureParm.h"
#include "駆動部/PIDCtrlParm.h"
#include "DrivenPart/S8.h"

namespace 駆動部
{
/**
 * PID曲率半径制御方式
 */
class PIDCurvatureCtrl
{
private:
	/**
	 * 偏差
	 */
	float deviation;

	float integratedDeviation;

	int lastMeasurementTime;

	PIDCurvatureCtrlParm pIDCurvatureCtrlParm;

public:
	/**
	 * 操作量を算出する
	 */
	DrivenPart::S8 PIDCurvatureCtrl_calcCurvatureCtrlVal(PIDCurvatureCtrl *this_PIDCurvatureCtrl);

	/**
	 * 曲率半径制御パラメータを取得する
	 */
	PIDCurvatureParm PIDCurvatureCtrl_getPIDCurvatureCtrlParm(PIDCurvatureCtrl *this_PIDCurvatureCtrl);

	/**
	 * 曲率半径制御パラメータを設定する
	 */
	void PIDCurvatureCtrl_setPIDCurvatureCtrlParm(PIDCurvatureCtrl *this_PIDCurvatureCtrl, PIDCtrlParm parm);

	void PIDCurvatureCtrl_init(PIDCurvatureCtrl *this_PIDCurvatureCtrl);

};

}  // namespace 駆動部
#endif

#ifndef 駆動部_P_I_D_CURVATURE_CTRL_PARM_H
#define 駆動部_P_I_D_CURVATURE_CTRL_PARM_H

#include <string>
#include <vector>
#include <list>
#include <iostream>
#include <assert.h>

namespace 駆動部
{
/**
 * PID曲率半径制御パラメータ
 */
class PIDCurvatureCtrlParm
{
private:
	/**
	 * 比例係数
	 */
	float cKp;

	/**
	 * 積分係数
	 */
	float cKi;

	/**
	 * 微分係数
	 */
	float cKd;


public:
	/**
	 * 比例係数を取得する
	 */
	float PIDCurvatureCtrlParm_getCKp(PIDCurvatureCtrlParm *this_PIDCurvatureCtrlParm);

	/**
	 * 比例係数を設定する
	 */
	float PIDCurvatureCtrlParm_setCKp(PIDCurvatureCtrlParm *this_PIDCurvatureCtrlParm, float parm);

	/**
	 * 積分係数を設定する
	 */
	float PIDCurvatureCtrlParm_getCKi(PIDCurvatureCtrlParm *this_PIDCurvatureCtrlParm);

	/**
	 * 積分係数を取得する
	 */
	float PIDCurvatureCtrlParm_setCKi(PIDCurvatureCtrlParm *this_PIDCurvatureCtrlParm, float parm);

	/**
	 * 微分係数を取得する
	 */
	float PIDCurvatureCtrlParm_getCKd(PIDCurvatureCtrlParm *this_PIDCurvatureCtrlParm);

	/**
	 * 積分係数を設定する
	 */
	float PIDCurvatureCtrlParm_setCKd(PIDCurvatureCtrlParm *this_PIDCurvatureCtrlParm, float parm);

};

}  // namespace 駆動部
#endif

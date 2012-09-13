#ifndef 駆動部_P_I_D_TAIL_ANGLE_CTRL_PARM_H
#define 駆動部_P_I_D_TAIL_ANGLE_CTRL_PARM_H

#include <string>
#include <vector>
#include <list>
#include <iostream>
#include <assert.h>

namespace 駆動部
{
/**
 * 尻尾角度PID制御パラメータ
 */
class PIDTailAngleCtrlParm
{
private:
	float tKp;

	float tKi;

	float tKd;


public:
	float PIDTailAngleParm_getTKp(PIDTailAngleCtrlParm *this_PIDTailAngleParm);

	void PIDTailAngleParm_setTKp(PIDTailAngleCtrlParm *this_PIDTailAngleParm, float parm);

	float PIDTailAngleParm_getTKi(PIDTailAngleCtrlParm *this_PIDTailAngleParm);

	void PIDTailAngleParm_setTKi(PIDTailAngleCtrlParm *this_PIDTailAngleParm, float parm);

	float PIDTailAngleParm_getTKd(PIDTailAngleCtrlParm *this_PIDTailAngleParm);

	void PIDTailAngleParm_setTKd(PIDTailAngleCtrlParm *this_PIDTailAngleParm, float parm);

};

}  // namespace 駆動部
#endif

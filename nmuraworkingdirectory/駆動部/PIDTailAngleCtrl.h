#ifndef �쓮��_P_I_D_TAIL_ANGLE_CTRL_H
#define �쓮��_P_I_D_TAIL_ANGLE_CTRL_H

#include <string>
#include <vector>
#include <list>
#include <iostream>
#include <assert.h>

#include "�쓮��/PIDTailAngleCtrlParm.h"
#include "�쓮��/PIDCtrlParm.h"
#include "DetectionPart/U16.h"

namespace �쓮��
{
/**
 * �K���p�xPID�������
 * 
 */
class PIDTailAngleCtrl
{
private:
	float deviation;

	float integratedDeviation;

	int lastMeasurementTime;

	PIDTailAngleCtrlParm pIDTailAngleCtrlParm;

public:
	void PIDTailAngleCtrl_calcTailAngleCtrlVal(PIDTailAngleCtrl *thisPIDTailAngleCtrl, DetectionPart::U16 targTailAngle, DetectionPart::U16 tailAngle);

	void PIDTailAngleCtrl_init(PIDTailAngleCtrl *this_PIDTailAngleCtrl);

	void PIDTailAngleCtrl_setPIDTailAngleCtrlParm(PIDTailAngleCtrl *this_PIDTailAngleCtrl, PIDCtrlParm parm);

	PIDCtrlParm PIDTailAngleCtrl_getPIDTailAngleCtrlParm(PIDTailAngleCtrl *this_PIDTailAngleCtrl);

};

}  // namespace �쓮��
#endif

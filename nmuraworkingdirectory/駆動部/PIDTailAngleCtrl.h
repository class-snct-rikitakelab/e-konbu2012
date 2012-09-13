#ifndef ‹ì“®•”_P_I_D_TAIL_ANGLE_CTRL_H
#define ‹ì“®•”_P_I_D_TAIL_ANGLE_CTRL_H

#include <string>
#include <vector>
#include <list>
#include <iostream>
#include <assert.h>

#include "‹ì“®•”/PIDTailAngleCtrlParm.h"
#include "‹ì“®•”/PIDCtrlParm.h"
#include "DetectionPart/U16.h"

namespace ‹ì“®•”
{
/**
 * K”öŠp“xPID§Œä•û®
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

}  // namespace ‹ì“®•”
#endif

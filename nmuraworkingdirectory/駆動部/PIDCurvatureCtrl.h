#ifndef ì®_P_I_D_CURVATURE_CTRL_H
#define ì®_P_I_D_CURVATURE_CTRL_H

#include <string>
#include <vector>
#include <list>
#include <iostream>
#include <assert.h>

#include "ì®/PIDCurvatureCtrlParm.h"
#include "ì®/PIDCurvatureParm.h"
#include "ì®/PIDCtrlParm.h"
#include "DrivenPart/S8.h"

namespace ì®
{
/**
 * PIDÈ¦¼a§äû®
 */
class PIDCurvatureCtrl
{
private:
	/**
	 * Î·
	 */
	float deviation;

	float integratedDeviation;

	int lastMeasurementTime;

	PIDCurvatureCtrlParm pIDCurvatureCtrlParm;

public:
	/**
	 * ìÊðZo·é
	 */
	DrivenPart::S8 PIDCurvatureCtrl_calcCurvatureCtrlVal(PIDCurvatureCtrl *this_PIDCurvatureCtrl);

	/**
	 * È¦¼a§äp[^ðæ¾·é
	 */
	PIDCurvatureParm PIDCurvatureCtrl_getPIDCurvatureCtrlParm(PIDCurvatureCtrl *this_PIDCurvatureCtrl);

	/**
	 * È¦¼a§äp[^ðÝè·é
	 */
	void PIDCurvatureCtrl_setPIDCurvatureCtrlParm(PIDCurvatureCtrl *this_PIDCurvatureCtrl, PIDCtrlParm parm);

	void PIDCurvatureCtrl_init(PIDCurvatureCtrl *this_PIDCurvatureCtrl);

};

}  // namespace ì®
#endif

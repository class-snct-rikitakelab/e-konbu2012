#ifndef ‹ì“®•”_P_I_D_LIGHT_VAL_CTRL_H
#define ‹ì“®•”_P_I_D_LIGHT_VAL_CTRL_H

#include <string>
#include <vector>
#include <list>
#include <iostream>
#include <assert.h>

#include "‹ì“®•”/LightValCtrlMethod.h"
#include "‹ì“®•”/S8.h"
#include "ŒŸo•”/U16.h"

namespace ‹ì“®•”
{
class PIDLightValCtrl : public LightValCtrlMethod
{
private:
	float deviation;

	float integratedDeviation;

	int lastMeasurementTime;


public:
	S8 PID_LightValCtrl_calcLightValCtrlVal(PIDLightValCtrl *this_PIDLightValCtrl, ŒŸo•”::U16 targLightVal, ŒŸo•”::U16 lightVal);

	void PIDLightValCtrl_PIDLightValCtrl_init(PIDLightValCtrl *this_PIDLightValCtrl);

};

}  // namespace ‹ì“®•”
#endif

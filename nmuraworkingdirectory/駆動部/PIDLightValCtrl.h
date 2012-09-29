#ifndef �쓮��_P_I_D_LIGHT_VAL_CTRL_H
#define �쓮��_P_I_D_LIGHT_VAL_CTRL_H

#include <string>
#include <vector>
#include <list>
#include <iostream>
#include <assert.h>

#include "�쓮��/LightValCtrlMethod.h"
#include "�쓮��/S8.h"
#include "���o��/U16.h"

namespace �쓮��
{
class PIDLightValCtrl : public LightValCtrlMethod
{
private:
	float deviation;

	float integratedDeviation;

	int lastMeasurementTime;


public:
	S8 PID_LightValCtrl_calcLightValCtrlVal(PIDLightValCtrl *this_PIDLightValCtrl, ���o��::U16 targLightVal, ���o��::U16 lightVal);

	void PIDLightValCtrl_PIDLightValCtrl_init(PIDLightValCtrl *this_PIDLightValCtrl);

};

}  // namespace �쓮��
#endif

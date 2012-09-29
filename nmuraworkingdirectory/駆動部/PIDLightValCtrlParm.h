#ifndef ‹ì“®•”_P_I_D_LIGHT_VAL_CTRL_PARM_H
#define ‹ì“®•”_P_I_D_LIGHT_VAL_CTRL_PARM_H

#include <string>
#include <vector>
#include <list>
#include <iostream>
#include <assert.h>

#include "‹ì“®•”/LightValCtrlParm.h"

namespace ‹ì“®•”
{
class PIDLightValCtrlParm : public LightValCtrlParm
{
private:
	float lKp;

	float lKi;

	float lKd;


public:
	float PIDLightValCtrlParm_getLKp(PIDLightValCtrlParm *this_PIDLightValCtrlParm);

	void PIDLightValCtrlParm_setLKp(PIDLightValCtrlParm *this_PIDLightValCtrlParm, float parm);

	float PIDLightValCtrlParm_getLKi(PIDLightValCtrlParm *this_PIDLightValCtrlParm);

	void PIDLightValCtrlParm_setLKi(PIDLightValCtrlParm *this_PIDLightValCtrlParm, float parm);

	float PIDLightValCtrlParm_getLKd(PIDLightValCtrlParm *this_PIDLightValCtrlParm);

	void PIDLightValCtrlParm_setLKd(PIDLightValCtrlParm *this_PIDLightValCtrlParm, float parm);

};

}  // namespace ‹ì“®•”
#endif

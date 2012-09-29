#ifndef �쓮��_P_I_D_LIGHT_VAL_CTRL_PARM_H
#define �쓮��_P_I_D_LIGHT_VAL_CTRL_PARM_H

#include <string>
#include <vector>
#include <list>
#include <iostream>
#include <assert.h>

#include "�쓮��/LightValCtrlParm.h"

namespace �쓮��
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

}  // namespace �쓮��
#endif

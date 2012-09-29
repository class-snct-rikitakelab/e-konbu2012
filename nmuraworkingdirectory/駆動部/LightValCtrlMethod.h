#ifndef �쓮��_LIGHT_VAL_CTRL_METHOD_H
#define �쓮��_LIGHT_VAL_CTRL_METHOD_H

#include <string>
#include <vector>
#include <list>
#include <iostream>
#include <assert.h>

#include "�쓮��/CtrlParm.h"
#include "���o��/U16.h"

namespace �쓮��
{
class LightValCtrlMethod
{
public:
	void LightValCtrlMethod_setCtrlParm(LightValCtrlMethod *this_LightValCtrlMethod, CtrlParm parm);

	virtual void LightValCtrlMethod_calcLightValCtrlVal(LightValCtrlMethod *this_LightValCtrlMethod, ���o��::U16 targLightVal, ���o��::U16 lightVal)=0;

	void LightValCtrlMethod_init(LightValCtrlMethod *this_LightValCtrlMethod);

};

}  // namespace �쓮��
#endif

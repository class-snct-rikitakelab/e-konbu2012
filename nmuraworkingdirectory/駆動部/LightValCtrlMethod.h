#ifndef ‹ì“®•”_LIGHT_VAL_CTRL_METHOD_H
#define ‹ì“®•”_LIGHT_VAL_CTRL_METHOD_H

#include <string>
#include <vector>
#include <list>
#include <iostream>
#include <assert.h>

#include "‹ì“®•”/CtrlParm.h"
#include "ŒŸo•”/U16.h"

namespace ‹ì“®•”
{
class LightValCtrlMethod
{
public:
	void LightValCtrlMethod_setCtrlParm(LightValCtrlMethod *this_LightValCtrlMethod, CtrlParm parm);

	virtual void LightValCtrlMethod_calcLightValCtrlVal(LightValCtrlMethod *this_LightValCtrlMethod, ŒŸo•”::U16 targLightVal, ŒŸo•”::U16 lightVal)=0;

	void LightValCtrlMethod_init(LightValCtrlMethod *this_LightValCtrlMethod);

};

}  // namespace ‹ì“®•”
#endif

#ifndef 駆動部_LIGHT_VAL_CTRL_METHOD_H
#define 駆動部_LIGHT_VAL_CTRL_METHOD_H

#include <string>
#include <vector>
#include <list>
#include <iostream>
#include <assert.h>

#include "駆動部/CtrlParm.h"
#include "検出部/U16.h"

namespace 駆動部
{
class LightValCtrlMethod
{
public:
	void LightValCtrlMethod_setCtrlParm(LightValCtrlMethod *this_LightValCtrlMethod, CtrlParm parm);

	virtual void LightValCtrlMethod_calcLightValCtrlVal(LightValCtrlMethod *this_LightValCtrlMethod, 検出部::U16 targLightVal, 検出部::U16 lightVal)=0;

	void LightValCtrlMethod_init(LightValCtrlMethod *this_LightValCtrlMethod);

};

}  // namespace 駆動部
#endif

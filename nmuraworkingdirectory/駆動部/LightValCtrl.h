#ifndef 駆動部_LIGHT_VAL_CTRL_H
#define 駆動部_LIGHT_VAL_CTRL_H

#include <string>
#include <vector>
#include <list>
#include <iostream>
#include <assert.h>

#include "駆動部/S8.h"
#include "駆動部/LightVal.h"
#include "駆動部/LightValCtrlMethod.h"
#include "検出部/U16.h"
#include "DrivenPart/LightValCtrl.h"

namespace 駆動部
{
class LightValCtrl
{
private:
	S8 LightValCtrlVal;

	LightVal lightVal;
	LightVal lightVal;
	LightValCtrlMethod lightValCtrlMethod;

public:
	void LightValCtrl_init(DrivenPart::LightValCtrl *this_LightValCtrl);

	void LightValCtrl_setTargLightVal(DrivenPart::LightValCtrl *this_LightValCtrl, 検出部::U16 targLightVal);

	S8 LightValCtrl_doLightValCtrl(DrivenPart::LightValCtrl *this_LightValCtrl);

};

}  // namespace 駆動部
#endif

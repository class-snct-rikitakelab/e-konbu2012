#ifndef ‹ì“®•”_LIGHT_VAL_CTRL_H
#define ‹ì“®•”_LIGHT_VAL_CTRL_H

#include <string>
#include <vector>
#include <list>
#include <iostream>
#include <assert.h>

#include "‹ì“®•”/S8.h"
#include "‹ì“®•”/LightVal.h"
#include "‹ì“®•”/LightValCtrlMethod.h"
#include "ŒŸo•”/U16.h"
#include "DrivenPart/LightValCtrl.h"

namespace ‹ì“®•”
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

	void LightValCtrl_setTargLightVal(DrivenPart::LightValCtrl *this_LightValCtrl, ŒŸo•”::U16 targLightVal);

	S8 LightValCtrl_doLightValCtrl(DrivenPart::LightValCtrl *this_LightValCtrl);

};

}  // namespace ‹ì“®•”
#endif

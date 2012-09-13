#ifndef �쓮��_LIGHT_VAL_CTRL_H
#define �쓮��_LIGHT_VAL_CTRL_H

#include <string>
#include <vector>
#include <list>
#include <iostream>
#include <assert.h>

#include "�쓮��/S8.h"
#include "�쓮��/LightVal.h"
#include "�쓮��/LightValCtrlMethod.h"
#include "���o��/U16.h"
#include "DrivenPart/LightValCtrl.h"

namespace �쓮��
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

	void LightValCtrl_setTargLightVal(DrivenPart::LightValCtrl *this_LightValCtrl, ���o��::U16 targLightVal);

	S8 LightValCtrl_doLightValCtrl(DrivenPart::LightValCtrl *this_LightValCtrl);

};

}  // namespace �쓮��
#endif

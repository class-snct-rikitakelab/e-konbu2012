#include "LightValCtrl.h"
#include "../Common/Factory.h"

void LightValCtrl_init(LightValCtrl *this_LightValCtrl)
{

}

void LightValCtrl_setTargLightVal(LightValCtrl *this_LightValCtrl, U16 targLightVal)
{
	LightVal_setTargLightVal(&mLightVal,targLightVal);
}

S8 LightValCtrl_doLightValCtrl(LightValCtrl *this_LightValCtrl)
{
	/*
	LightVal_getLightValue();
	LightVal_getTargLight();
	PIDLightValCtrl_calcLightValCtrlVal();
*/
	return 0;
}

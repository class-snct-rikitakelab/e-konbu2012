#include "LightVal.h"

void LightVal_init(LightVal * this_LightVal)
{
	this_LightVal->targetLightVal = 0;
}

//輝度値を取得する
U16 LightVal_getLightVal(LightVal * this_LightVal)
{
	return LightSensor_getLightSensorVal(0);
}

//目標輝度値を取得する
U16 LightVal_getTargLightVal(LightVal * this_LightVal)
{
	return this_LightVal->targetLightVal;
}

//目標輝度値を設定する
void LightVal_setTargLightVal(LightVal * this_LightVal,U16 targLightVal)
{
	this_LightVal->targetLightVal = targLightVal;
}
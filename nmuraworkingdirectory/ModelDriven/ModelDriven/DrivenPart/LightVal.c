#include "LightVal.h"

/*
 *	LightVal.c
 *	輝度値クラス
 */

//初期化メソッド
void LightVal_init(LightVal * this_LightVal)
{
	this_LightVal->targetLightVal = 0;
}

//輝度値取得メソッド
U16 LightVal_getLightVal(LightVal * this_LightVal)
{
	return LightSensor_getLightSensorVal(&mLightSensor);
}

//目標輝度値メソッド
U16 LightVal_getTargLightVal(LightVal * this_LightVal)
{
	return this_LightVal->targetLightVal;
}

//目標輝度値設定メソッド
void LightVal_setTargLightVal(LightVal * this_LightVal,U16 targLightVal)
{
	this_LightVal->targetLightVal = targLightVal;
}
#include "LightVal.h"

/*
 *	LightVal.c
 *	�P�x�l�N���X
 */

//���������\�b�h
void LightVal_init(LightVal * this_LightVal)
{
	this_LightVal->targetLightVal = 0;
}

//�P�x�l�擾���\�b�h
U16 LightVal_getLightVal(LightVal * this_LightVal)
{
	return LightSensor_getLightSensorVal(&mLightSensor);
}

//�ڕW�P�x�l���\�b�h
U16 LightVal_getTargLightVal(LightVal * this_LightVal)
{
	return this_LightVal->targetLightVal;
}

//�ڕW�P�x�l�ݒ胁�\�b�h
void LightVal_setTargLightVal(LightVal * this_LightVal,U16 targLightVal)
{
	this_LightVal->targetLightVal = targLightVal;
}
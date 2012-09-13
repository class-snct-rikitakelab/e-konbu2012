#include "LightVal.h"

void LightVal_init(LightVal * this_LightVal)
{
	this_LightVal->targetLightVal = 0;
}

//�P�x�l���擾����
U16 LightVal_getLightVal(LightVal * this_LightVal)
{
	return LightSensor_getLightSensorVal(0);
}

//�ڕW�P�x�l���擾����
U16 LightVal_getTargLightVal(LightVal * this_LightVal)
{
	return this_LightVal->targetLightVal;
}

//�ڕW�P�x�l��ݒ肷��
void LightVal_setTargLightVal(LightVal * this_LightVal,U16 targLightVal)
{
	this_LightVal->targetLightVal = targLightVal;
}
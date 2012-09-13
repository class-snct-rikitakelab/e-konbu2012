
#include "LightSensor.h"

void LightSensor_init(LightSensor *this_LightSensor){

}
S8 LightSensor_getLightSensorVal(LightSensor *this_LightSensor)
{
	return ecrobot_get_light_sensor(NXT_PORT_S3);
}

S8 LightSensor_getLPFLightSensorVal(LightSensor *this_LightSensor)
{
	return 0;
}




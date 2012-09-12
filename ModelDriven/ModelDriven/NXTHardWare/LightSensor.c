
#include "LightSensor.h"


S8 LightSensor_getLightSensorVal(LightSensor *this_LightSensor)
{
	return ecrobot_get_light_sensor(NXT_PORT_S3);
}

S8 LightSensor_getLPFLightSensorVal(LightSensor *this_LightSensor)
{
	return 0;
}


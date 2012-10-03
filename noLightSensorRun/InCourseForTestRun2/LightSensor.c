
#include "LightSensor.h"
#include "Factory.h"

void LightSensor_init(LightSensor *this_LightSensor){

}
U16 LightSensor_getLightSensorVal(LightSensor *this_LightSensor)
{
	return ecrobot_get_light_sensor(NXT_PORT_S3);
}

U16 LightSensor_getLPFLightSensorVal(LightSensor *this_LightSensor)
{
	static U16 lightBuf=0;
	float A=0.5;

	

	U16 resLight = A*LightSensor_getLightSensorVal(&mLightSensor) + (1-A)*lightBuf;
	
	lightBuf= LightSensor_getLightSensorVal(&mLightSensor);
	return resLight;
}





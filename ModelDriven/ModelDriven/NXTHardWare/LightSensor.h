#ifndef __LightSensor
#define __LightSensor

#include "ecrobot_interface.h"
/**
 * 光センサ
 */
typedef struct{

}LightSensor;

void LightSensor_init(LightSensor *this_LightSensor);
	/**
	 * 計測する
	 */
U16 LightSensor_getLightSensorVal(LightSensor *this_LightSensor);
U16 LightSensor_getLPFLightSensorVal(LightSensor *this_LightSensor);

#endif

#include "ecrobot_interface.h"
/**
 * ŒõƒZƒ“ƒT
 */
typedef struct GyroSensor{

}LightSensor;

void LightSensor_init(LightSensor *this_LightSensor);
	/**
	 * Œv‘ª‚·‚é
	 */
S8 LightSensor_getLightSensorVal(LightSensor *this_LightSensor);
S8 LightSensor_getLPFLightSensorVal(LightSensor *this_LightSensor);
#ifndef __LightSensor
#define __LightSensor

#include "ecrobot_interface.h"
/**
 * ���Z���T
 */
typedef struct{

}LightSensor;

void LightSensor_init(LightSensor *this_LightSensor);
	/**
	 * �v������
	 */
U16 LightSensor_getLightSensorVal(LightSensor *this_LightSensor);
U16 LightSensor_getLPFLightSensorVal(LightSensor *this_LightSensor);

#endif

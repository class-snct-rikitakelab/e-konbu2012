#include "ecrobot_interface.h"
/**
 * ���Z���T
 */
typedef struct GyroSensor{

}LightSensor;

void LightSensor_init(LightSensor *this_LightSensor);
	/**
	 * �v������
	 */
S8 LightSensor_getLightSensorVal(LightSensor *this_LightSensor);
S8 LightSensor_getLPFLightSensorVal(LightSensor *this_LightSensor);
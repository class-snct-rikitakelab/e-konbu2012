#include "ecrobot_interface.h"
/**
 * 光センサ
 */
typedef struct GyroSensor{

}LightSensor;

	/**
	 * 計測する
	 */
S8 LightSensor_getLightSensorVal(LightSensor *this_LightSensor);
S8 LightSensor_getLPFLightSensorVal(LightSensor *this_LightSensor);
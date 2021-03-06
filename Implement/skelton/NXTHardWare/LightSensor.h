
#include <string>
#include <vector>
#include <list>
#include <iostream>
#include <assert.h>

#include "DrivenPart/S8.h"

namespace NXTHardWare
{
/**
 * 光センサ
 */
class LightSensor
{
public:
	void LightSensor_init(LightSensor *this_LightSensor);

	/**
	 * 計測する
	 */
	DrivenPart::S8 LightSensor_getLightSensorVal(LightSensor *this_LightSensor);

	DrivenPart::S8 LightSensor_getLPFLightSensorVal(LightSensor *this_LightSensor);

};

}  // namespace NXTHardWare

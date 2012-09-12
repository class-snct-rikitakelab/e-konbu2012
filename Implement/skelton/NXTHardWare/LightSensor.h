
#include <string>
#include <vector>
#include <list>
#include <iostream>
#include <assert.h>

#include "DrivenPart/S8.h"

namespace NXTHardWare
{
/**
 * ���Z���T
 */
class LightSensor
{
public:
	void LightSensor_init(LightSensor *this_LightSensor);

	/**
	 * �v������
	 */
	DrivenPart::S8 LightSensor_getLightSensorVal(LightSensor *this_LightSensor);

	DrivenPart::S8 LightSensor_getLPFLightSensorVal(LightSensor *this_LightSensor);

};

}  // namespace NXTHardWare

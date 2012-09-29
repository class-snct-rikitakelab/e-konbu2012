
#include <string>
#include <vector>
#include <list>
#include <iostream>
#include <assert.h>

#include "DetectionPart/S32.h"

namespace NXTHardWare
{
/**
 * �����g�Z���T
 */
class SonorSensor
{
public:
	void SonorSensor_init(SonorSensor *this_SonorSensor);

	/**
	 * �v������
	 */
	DetectionPart::S32 SonorSensor_getSonor(SonorSensor *this_SonorSensor);

};

}  // namespace NXTHardWare

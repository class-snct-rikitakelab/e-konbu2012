
#include <string>
#include <vector>
#include <list>
#include <iostream>
#include <assert.h>

#include "DetectionPart/U16.h"

namespace NXTHardWare
{
/**
 * �W���C���Z���T
 */
class GyroSensor
{
public:
	void GyroSensor_init(GyroSensor *this_GyroSensor);

	/**
	 * �p���x���擾����
	 */
	DetectionPart::U16 GyroSensor_getGyroAngVelocity(GyroSensor *this_GyroSensor);

};

}  // namespace NXTHardWare

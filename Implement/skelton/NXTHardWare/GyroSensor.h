
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
	/**
	 * �p���x���擾����
	 */
	DetectionPart::U16 GyroSensor_getGyroAngVelocity(GyroSensor *this_GyroSensor);

};

}  // namespace NXTHardWare

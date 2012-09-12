
#include <string>
#include <vector>
#include <list>
#include <iostream>
#include <assert.h>

#include "DetectionPart/U16.h"
#include "DetectionPart/Notice.h"
#include "NXTHardWare/GyroSensor.h"

namespace DetectionPart
{
/**
 * �X�����o
 */
class Gradient
{
private:
	/**
	 * �w��p���x
	 */
	U16 TargetGradient;

	NXTHardWare::GyroSensor gyroSensor;
	Notice notice;

public:
	/**
	 * �w��p���x�����m����
	 */
	bool Gradient_dectTargGradient(Gradient *this_Gradient);

};

}  // namespace DetectionPart

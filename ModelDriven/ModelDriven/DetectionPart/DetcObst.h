
#include <string>
#include <vector>
#include <list>
#include <iostream>
#include <assert.h>

#include "DetectionPart/S32.h"
#include "DetectionPart/Notice.h"
#include "NXTHardWare/UltrasonicSensor.h"

namespace DetectionPart
{
/**
 * ��Q�����o
 */
class DetcObst
{
private:
	/**
	 * ���m�Ώۋ���
	 */
	S32 detcDist;

	Notice notice;
	NXTHardWare::UltrasonicSensor ultrasonicSensor;

public:
	/**
	 * �w�苗�������m����
	 */
	bool DetcObstacle_DetcObst(DetcObst *this_DetcObst);

};

}  // namespace DetectionPart

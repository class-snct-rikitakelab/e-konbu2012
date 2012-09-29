
#include <string>
#include <vector>
#include <list>
#include <iostream>
#include <assert.h>

#include "DetectionPart/S32.h"
#include "DetectionPart/Notice.h"
#include "NXTHardWare/SonorSensor.h"

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
	NXTHardWare::SonorSensor sonorSensor;

public:
	void DetcObst_init(DetcObst *this_DetcObst);

	/**
	 * �w�苗�������m����
	 */
	bool DetcObstacle_DetcObst(DetcObst *this_DetcObst);

};

}  // namespace DetectionPart


#include <string>
#include <vector>
#include <list>
#include <iostream>
#include <assert.h>

#include "DetectionPart/Notice.h"
#include "NXTHardWare/MotorEncoder.h"

namespace DetectionPart
{
/**
 * �ړ�����
 */
class RunDist
{
private:
	/**
	 * �w��ړ�����
	 */
	float targRunDist;

	Notice notice;
	NXTHardWare::MotorEncoder motorEncoder;

public:
	bool RunDist_dectTargRunDist(RunDist *this_RunDist);

};

}  // namespace DetectionPart

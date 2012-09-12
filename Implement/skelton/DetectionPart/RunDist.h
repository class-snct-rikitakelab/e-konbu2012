
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
 * ˆÚ“®‹——£
 */
class RunDist
{
private:
	/**
	 * Žw’èˆÚ“®‹——£
	 */
	float targRunDist;

	Notice notice;
	NXTHardWare::MotorEncoder motorEncoder;

public:
	void RunDist_init(RunDist *this_RunDist);

	bool RunDist_dectTargRunDist(RunDist *this_RunDist);

};

}  // namespace DetectionPart

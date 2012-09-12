
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
 * 障害物検出
 */
class DetcObst
{
private:
	/**
	 * 検知対象距離
	 */
	S32 detcDist;

	Notice notice;
	NXTHardWare::UltrasonicSensor ultrasonicSensor;

public:
	/**
	 * 指定距離を検知する
	 */
	bool DetcObstacle_DetcObst(DetcObst *this_DetcObst);

};

}  // namespace DetectionPart


#include <string>
#include <vector>
#include <list>
#include <iostream>
#include <assert.h>

#include "DetectionPart/Notice.h"
#include "DetectionPart/LocationInfo.h"

namespace DetectionPart
{
/**
 * 旋回角度
 */
class TurnAngle
{
private:
	/**
	 * 指定方位
	 */
	float targAngle;

	Notice notice;
	LocationInfo locationInfo;

public:
	void TurnAngle_init(TurnAngle *this_TurnAngle);

	bool TurnAngle_detcTurnAngle(TurnAngle *this_TurnAngle, TurnAngle TurnAngle);

};

}  // namespace DetectionPart

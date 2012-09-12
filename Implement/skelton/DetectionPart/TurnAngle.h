
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
 * ù‰ñŠp“x
 */
class TurnAngle
{
private:
	/**
	 * w’è•ûˆÊ
	 */
	float targAngle;

	Notice notice;
	LocationInfo locationInfo;

public:
	bool TurnAngle_detcTurnAngle(TurnAngle *this_TurnAngle, TurnAngle TurnAngle);

};

}  // namespace DetectionPart

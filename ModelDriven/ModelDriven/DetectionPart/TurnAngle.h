#ifndef __TurnAngle
#define __TurnAngle

#include "DetectionPart/Notice.h"
#include "DetectionPart/LocationInfo.h"
#include "../Common/Common.h"

/**
 * ù‰ñŠp“x
 */
typedef struct {
		/**
	 * w’è•ûˆÊ
	 */
	float targAngle;
}TurnAngle;


void TurnAngle_init(TurnAngle *this_TurnAngle);

BOOL TurnAngle_detcTurnAngle(TurnAngle *this_TurnAngle);


#endif


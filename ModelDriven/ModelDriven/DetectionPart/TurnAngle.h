#ifndef __TurnAngle
#define __TurnAngle

#include "DetectionPart/Notice.h"
#include "DetectionPart/LocationInfo.h"
#include "../Common/Common.h"

/**
 * ����p�x
 */
typedef struct {
		/**
	 * �w�����
	 */
	float targAngle;
	float bufAnlge;
}TurnAngle;


void TurnAngle_init(TurnAngle *this_TurnAngle);

BOOL TurnAngle_detcTurnAngle(TurnAngle *this_TurnAngle);

void TurnAngle_reset(TurnAngle *this_TurnAngle);

#endif


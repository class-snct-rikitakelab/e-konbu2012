#ifndef __TurnAngle
#define __TurnAngle

#include "DetectionPart/Notice.h"
#include "DetectionPart/LocationInfo.h"


/**
 * ����p�x
 */
typedef struct {
		/**
	 * �w�����
	 */
	float targAngle;
}TurnAngle;


void TurnAngle_init(TurnAngle *this_TurnAngle);

bool TurnAngle_detcTurnAngle(TurnAngle *this_TurnAngle, TurnAngle TurnAngle);


#endif


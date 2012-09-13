#ifndef __RunDist
#define __RunDist


#include "DetectionPart/Notice.h"
#include "NXTHardWare/MotorEncoder.h"
#include "../Common/Common.h"


/**
 * ˆÚ“®‹——£
 */
typedef struct {
/**
	 * Žw’èˆÚ“®‹——£
	 */
	float targRunDist;
}RunDist;

	
	void RunDist_init(RunDist *this_RunDist);

	BOOL RunDist_dectTargRunDist(RunDist *this_RunDist);


#endif

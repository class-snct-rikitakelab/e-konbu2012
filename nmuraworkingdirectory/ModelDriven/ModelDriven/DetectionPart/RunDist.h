#ifndef __RunDist
#define __RunDist


#include "../NXTHardWare/MotorEncoder.h"
#include "../Common/Common.h"


/**
 * ˆÚ“®‹——£
 */
typedef struct {
/**
	 * Žw’èˆÚ“®‹——£
	 */
	float targRunDist;
	float leftCountBuf;
	float rightCountBuf;
}RunDist;

	
	void RunDist_init(RunDist *this_RunDist);

	BOOL RunDist_dectTargRunDist(RunDist *this_RunDist);
	void RunDist_resetRunDist(RunDist *this_RunDist);

#endif

#ifndef __DetcObst
#define __DetcObst


#include "../DetectionPart/Notice.h"
#include "../NXTHardWare/SonorSensor.h"
#include "../Common/Common.h"


/**
 * 障害物検出
 */
typedef struct { 
	/**
	 * 検知対象距離
	 */
	S32 dectDist;
}DetcObst;

void DetcObst_init(DetcObst *this_DetcObst);

	/**
	 * 指定距離を検知する
	 */
	BOOL DetcObstacle_DetcObst(DetcObst *this_DetcObst);

#endif


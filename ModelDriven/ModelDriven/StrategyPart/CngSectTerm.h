#ifndef __CngSectTerm
#define __CngSectTerm
#include "../Common/Common.h"
#include "../Common/SectName.h"

/**
 * ‹æŠÔØ‘ÖğŒ
 */
typedef struct  {
	/**
	 * Ø‘ÖğŒ
	 */
	SectName sectName;
	BOOL jBT;
	int targDist;
	BOOL jlineEdge;
U16 targetGradient;
	S32 targTime;
	float targXCoo;
	float targYCoo;
	S8 makerThreshold;
	int targObstDist;
	int targTurnAngle;
	int targBatteryVol;
}CngSectTerm;

#endif

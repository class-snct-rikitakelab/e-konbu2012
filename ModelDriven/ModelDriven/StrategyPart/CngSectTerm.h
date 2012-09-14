#ifndef __CngSectTerm
#define __CngSectTerm
#include "../Common/Common.h"
#include "../Common/SectName.h"

/**
 * ��Ԑؑ֏���
 */
typedef struct  {
	/**
	 * �ؑ֏���
	 */
	SectName sectName;
	BOOL jBT;
	int targDist;
	int lineEdgeThreshold;
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

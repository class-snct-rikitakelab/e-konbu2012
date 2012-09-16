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
	SectName nextSectName;
	BOOL jBT;
	int targDist;
	BOOL jLineEdge;
U16 targGradient;
	S32 targTime;
	float targXCoo;
	float targYCoo;
	U16 makerThreshold;
	int targObstDist;
	int targTurnAngle;
	int targBatteryVol;
}CngSectTerm;

void CngSectTerm_init(CngSectTerm *this_CngSectTerm,SectName nextSectName1,BOOL jBT1,int targDist1,BOOL jLineEdge1,U16 targGradient1,S32 targTime1,float targXCoo1,float targYCoo1,U16 makerhreshold1,int targObstDist1,int targTurnAngle1,int targBatteryVol1,
	SectName nextSectName2,BOOL jBT2,int targDist2,BOOL jLineEdge2,U16 targGradient2,S32 targTime2,float targXCoo2,float targYCoo2,U16 makerhreshold2,int targObstDist2,int targTurnAngle2,int targBatteryVol2,
	SectName nextSectName3,BOOL jBT3,int targDist3,BOOL jLineEdge3,U32 targGradient3,S32 targTime3,float targXCoo3,float targYCoo3,U32 makerhreshold3,int targObstDist3,int targTurnAngle3,int targBatteryVol3);

#endif

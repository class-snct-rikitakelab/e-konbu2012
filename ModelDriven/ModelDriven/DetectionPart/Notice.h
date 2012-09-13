#ifndef __Notice
#define __Notice

#include "../StrategyPart/CngSectTerm.h"
#include "../DetectionPart/RunDist.h"
#include "../DetectionPart/TurnAngle.h"
#include "../DetectionPart/Coordinates.h"
#include "../DetectionPart/DetcObst.h"
#include "../DetectionPart/RunStartSigRecv.h"
#include "../DetectionPart/LineEdge.h"
#include "../DetectionPart/DetcMarker.h"
#include "../DetectionPart/Gradient.h"
#include "../DetectionPart/Timer.h"
#include "../DetectionPart/Battery.h"
#include "../DirectorPart/CngSectLisnner.h"

/**
 * �ʒm��
 */
typedef struct {	
//	�ʒm���� noticeTerm;

}Notice;

/*
typedef struct {
SectID;
jLineEdge;
jBT;
targDist;
targbattery;
targGradient;
targTime;
targXCoo;
targYCoo;
jMarker;
targObstDist;
targTurnAngle;

}cngSectTerm;
*/

void Notice_init(Notice *this_Notice);

	/**
	 * ��Ԑؑ֏����w��
	 */
void Notice_setCngSectTerm(Notice *this_Notice,CngSectTerm term);

	/**
	 * �ʒm���f
	 */
extern void Notice_judgeNotice(Notice *this_Notice);

#endif


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

namespace DetectionPart
{
/**
 * 通知器
 */
typedef struct {	
	通知条件 noticeTerm;

}Notice;


void Notice_init(Notice *this_Notice);

	/**
	 * 区間切替条件指定
	 */
void Notice_setCngSectTerm(Notice *this_Notice,CngSectTerm term);

	/**
	 * 通知判断
	 */
void Notice_judgeNotice(Notice *this_Notice);

#endif
#ifndef __Running
#define __Running

#include "../StrategyPart/Sector.h"
#include "../StrategyPart/TargetDrivenParm.h"
#include "../DrivenPart/TailAngleCtrl.h"
#include "../DrivenPart/RobotDrivenDirect.h"
#include "../DetectionPart/Notice.h"
#include "../DetectionPart/RunStartSigRecv.h"
#include "../DirectorPart/CngSectLisnner.h"
#include "../Common/SectName.h"

/**
 * 走行
 */

typedef struct {
	int currentSect;
}Running; 
	/**
	 * 現在区間
	 */
void Running_init(Running *this_Running);

	/**
	 * 現在区間を走行する
	 */
void Running_runCurrentSect(Running *this_Running);

	/**
	 * 次の区間に切り替える
	 */
void Running_cngNextSect(Running *this_Running,SectName nextSectname);


#endif



#ifndef __Running
#define __Running

#include "../StrategyPart/Sector.h"
#include "../StrategyPart/TargetDrivenParm.h"
#include "../DrivenPart/TailAngleCtrl.h"
#include "../DrivenPart/RobotDrivenDirect.h"
#include "../DetectionPart/Notice.h"
#include "../DetectionPart/RunStartSigRecv.h"
#include "../DirectorPart/CngSectLisnner.h"


/**
 * ‘–s
 */

typedef struct {
	int currentSect;
}Running; 
	/**
	 * Œ»İ‹æŠÔ
	 */
void Running_init(Running *this_Running);

	/**
	 * Œ»İ‹æŠÔ‚ğ‘–s‚·‚é
	 */
void Running_runCurrentSect(Running *this_Running);

	/**
	 * Ÿ‚Ì‹æŠÔ‚ÉØ‚è‘Ö‚¦‚é
	 */
//void Running_cngNextSect(Running *this_Running,Notice checkResult);


#endif



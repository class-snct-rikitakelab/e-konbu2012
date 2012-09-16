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
 * ���s
 */

typedef struct {
	Sector currentSect;
}Running; 
	/**
	 * ���݋��
	 */
void Running_init(Running *this_Running);

	/**
	 * ���݋�Ԃ𑖍s����
	 */
void Running_runCurrentSect(Running *this_Running);

	/**
	 * ���̋�Ԃɐ؂�ւ���
	 */
void Running_cngNextSect(Running *this_Running,SectName nextSectname);


#endif



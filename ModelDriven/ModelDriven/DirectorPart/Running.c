#include "Running.h"
#include "../Common/Factory.h"


void Running_init(Running *this_Running){
	this_Running->currentSect=0;
}

	/**
	 * ���݋�Ԃ𑖍s����
	 */
void Running_runCurrentSect(Running *this_Running){
	RobotDrivenDirect_directDrive(&mRobotDrivenDirect);
}

	/**
	 * ���̋�Ԃɐ؂�ւ���
	 */
void Running_cngNextSect(Running *this_Running,SectName nxetSectName){
	Sector_getNextSect(&mSector);

}

	
#include "Running.h"
#include "../Common/Factory.h"


void Running_init(Running *this_Running){
	this_Running->currentSect=0;
}

	/**
	 * 現在区間を走行する
	 */
void Running_runCurrentSect(Running *this_Running){
	RobotDrivenDirect_directDrive(&mRobotDrivenDirect);
}

	/**
	 * 次の区間に切り替える
	 */
void Running_cngNextSect(Running *this_Running,SectName nxetSectName){
	Sector_getNextSect(&mSector);

}

	
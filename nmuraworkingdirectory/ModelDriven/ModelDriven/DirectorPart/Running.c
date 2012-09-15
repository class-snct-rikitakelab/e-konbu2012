#include "Running.h"
#include "../Common/Factory.h"


void Running_init(Running *this_Running){
	this_Running->currentSect=0;
}

	/**
	 * Œ»İ‹æŠÔ‚ğ‘–s‚·‚é
	 */
void Running_runCurrentSect(Running *this_Running){
	RobotDrivenDirect_directDrive(&mRobotDrivenDirect);
}

	/**
	 * Ÿ‚Ì‹æŠÔ‚ÉØ‚è‘Ö‚¦‚é
	 */
void Running_cngNextSect(Running *this_Running,SectName nxetSectName){
	Sector_getNextSect(&mSector);

}

	
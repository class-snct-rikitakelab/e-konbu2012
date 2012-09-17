#include "Running.h"
#include "../Common/Factory.h"


void Running_init(Running *this_Running){
	this_Running->currentSect=idleSector; //構造体のフルコピー
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
void Running_cngNextSect(Running *this_Running,SectName nextSectName){
	//次の区間を名前を指定して取得
	this_Running->currentSect = Sector_getNextSect(&this_Running->currentSect,nextSectName);
	
	//新しい区間の駆動パラメータを取得
	TargetDrivenParm drivenParm = Sector_getTargDrienParm(&(this_Running->currentSect));

	//駆動パラメータをセット
	RobotDrivenDirect_setDriveParm(&mRobotDrivenDirect,drivenParm);

	//新しい区間の区間切替条件を取得しセット
	CngSectTerm  *term = Sector_getCngSectTerm(&(this_Running->currentSect));
	Notice_setCngSectTerm(&mNotice,term);

}

	
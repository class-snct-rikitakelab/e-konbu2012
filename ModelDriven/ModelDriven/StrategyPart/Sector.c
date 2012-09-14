
#include "Sector.h"

void Sector_init(Sector *this_Sector){
	//test code
	this_Sector->cngTerm[0].jBT=true;
	this_Sector->cngTerm[0].targDist=30;
}

	/**
	 * 区間IDを取得する
	 */
int Sector_getSectID(Sector *this_Sector){
	return 0;
}

	/**
	 * 次区間への切替条件取得
	 */
CngSectTerm Sector_getCngSectTerm(Sector *this_Sector){
	return this_Sector->cngTerm[0];
}

	/**
	 * 次区間を取得する
	 */
Sector Sector_getNextSect(Sector *this_Sector){
	return *this_Sector; //test code
}

	/**
	 * 目標駆動パラメータを取得する
	 */
TargetDrivenParm Sector_getTargDrienParm(Sector *this_Sector){

	return this_Sector->targetDrvenParm;
}


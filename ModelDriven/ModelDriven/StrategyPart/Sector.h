#ifndef __Sector
#define __Sector

#include "../StrategyPart/TargetDrivenParm.h"
#include "../StrategyPart/CngSectTerm.h"
#include "../Common/SectName.h"

/**
 * 区間
 */
typedef struct  {
		/**
	 * 区間ID
	 */

	SectName sectName;

	TargetDrivenParm targetDrvenParm;
	CngSectTerm cngTerm[CNG_TERM_MAX_NUM];
	//Sector 次区間[];

}Sector;


	void Sector_init(Sector *this_Sector);

	/**
	 * 区間IDを取得する
	 */
	int Sector_getSectID(Sector *this_Sector);

	/**
	 * 次区間への切替条件取得
	 */
	CngSectTerm Sector_getCngSectTerm(Sector *this_Sector);

	/**
	 * 次区間を取得する
	 */
	Sector Sector_getNextSect(Sector *this_Sector);

	/**
	 * 目標駆動パラメータを取得する
	 */
	TargetDrivenParm Sector_getTargDrienParm(Sector *this_Sector);

#endif


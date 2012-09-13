
#include "Sector.h"

void Sector_init(Sector *this_Sector);

	/**
	 * 区間IDを取得する
	 */
	int Sector_getSectID(Sector *this_Sector);

	/**
	 * 次区間切替条件取得
	 */
	CngSectTerm Sector_getNextCngSectTerm(Sector *this_Sector);

	/**
	 * 次区間を取得する
	 */
	Sector Sector_GetNextSect(Sector *this_Sector);

	/**
	 * 目標駆動パラメータを取得する
	 */
	TargetDrivenParm Sector_getTargDrienParm(Sector *this_Sector);


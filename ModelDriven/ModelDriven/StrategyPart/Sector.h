
#include <string>
#include <vector>
#include <list>
#include <iostream>
#include <assert.h>

#include "StrategyPart/TargetDrivenParm.h"
#include "StrategyPart/CngSectTerm.h"

namespace StrategyPart
{
/**
 * 区間
 */
class Sector
{
private:
	/**
	 * 区間ID
	 */
	int sectID;

	TargetDrivenParm 目標駆動パラメータ;
	CngSectTerm 切替条件[];
	Sector 次区間[];

public:
	/**
	 * 区間IDを取得する
	 */
	int Sector_getSectID();

	/**
	 * 次区間切替条件取得
	 */
	CngSectTerm Sector_getNextCngSectTerm();

	/**
	 * 次区間を取得する
	 */
	Sector Sector_GetNextSect();

	/**
	 * 目標駆動パラメータを取得する
	 */
	TargetDrivenParm Sector_getTargDrienParm();

};

}  // namespace StrategyPart

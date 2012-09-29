#ifndef __Battery
#define __Battery
#include "../Common/Common.h"

/**
 * 電圧計
 */
typedef struct {
	/**
	 * バッテリ閾値
	 */
	int batteryThereshold;

}Battery;

	void Battery_init(Battery *this_Battery);

	BOOL Battery_dectBatteryThreShold(Battery *this_Battery);

#endif



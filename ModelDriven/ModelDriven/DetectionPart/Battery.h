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
	U16 targReductRang;
	U16 initVol;

}Battery;

	void Battery_init(Battery *this_Battery);

	BOOL Battery_dectRedct(Battery *this_Battery);

#endif



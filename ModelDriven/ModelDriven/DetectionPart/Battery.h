#ifndef __Battery
#define __Battery
#include "../Common/Common.h"

/**
 * �d���v
 */
typedef struct {
	/**
	 * �o�b�e��臒l
	 */
	U16 targReductRang;
	U16 initVol;

}Battery;

	void Battery_init(Battery *this_Battery);

	BOOL Battery_dectRedct(Battery *this_Battery);

#endif



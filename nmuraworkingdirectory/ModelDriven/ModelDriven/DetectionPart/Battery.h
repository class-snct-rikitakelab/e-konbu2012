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
	int batteryThereshold;

}Battery;

	void Battery_init(Battery *this_Battery);

	BOOL Battery_dectBatteryThreShold(Battery *this_Battery);

#endif



#ifndef __Timer
#define __Timer
#include "../Common/Common.h"

/**
 * ���Ԍv
 */
typedef struct {
	int targTimer;
}
Timer;

	void Timer_init(Timer *this_Timer);

	/**
	 * �w�莞�Ԃ����m����
	 */
	BOOL Timer_detcPassTargTimer(Timer *this_Timer);

#endif

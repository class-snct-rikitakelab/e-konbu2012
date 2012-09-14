#ifndef __Timer
#define __Timer
#include "../Common/Common.h"

/**
 * ŠÔŒv
 */
typedef struct {
	int targTimer;
}
Timer;

	void Timer_init(Timer *this_Timer);

	/**
	 * w’èŠÔ‚ğŒŸ’m‚·‚é
	 */
	BOOL Timer_detcPassTargTimer(Timer *this_Timer);

#endif

#ifndef __Timer
#define __Timer
#include "../Common/Common.h"

/**
 * 時間計
 */
typedef struct {
	int targTimer;
}
Timer;

	void Timer_init(Timer *this_Timer);

	/**
	 * 指定時間を検知する
	 */
	BOOL Timer_detcPassTargTimer(Timer *this_Timer);

#endif

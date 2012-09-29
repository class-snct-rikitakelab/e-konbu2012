#ifndef __Timer
#define __Timer
#include "../Common/Common.h"
#include "ecrobot_interface.h"
/**
 * 時間計 時間の指定はms単位
 */
typedef struct {
	int targTimer;
	int timerbuf;
}
Timer;

	void Timer_init(Timer *this_Timer);

	/**
	 * 指定時間経過を検知する
	 */
	BOOL Timer_detcPassTargTimer(Timer *this_Timer);
	
	/**
 * 区間走行経過時間をリセット
**/
	void Timer_reset(Timer *this_Timer);

#endif

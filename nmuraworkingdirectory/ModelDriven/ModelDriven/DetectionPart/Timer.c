
#include "Timer.h"
void Timer_init(Timer *this_Timer){
	this_Timer->timerbuf = 0;
	this_Timer->targTimer=0;
}

	/**
	 * �w�莞�Ԃ����m����
	 */
BOOL Timer_detcPassTargTimer(Timer *this_Timer){
	BOOL res = false;
	
	
	if(systick_get_ms() - this_Timer->timerbuf > this_Timer->targTimer){
		res = true;
	}
	return res;
}

void Timer_reset(Timer *this_Timer){
	this_Timer->timerbuf=systick_get_ms();
}
#ifndef __Timer
#define __Timer
#include "../Common/Common.h"
#include "ecrobot_interface.h"
/**
 * ���Ԍv ���Ԃ̎w���ms�P��
 */
typedef struct {
	int targTimer;
	int timerbuf;
}
Timer;

	void Timer_init(Timer *this_Timer);

	/**
	 * �w�莞�Ԍo�߂����m����
	 */
	BOOL Timer_detcPassTargTimer(Timer *this_Timer);
	
	/**
 * ��ԑ��s�o�ߎ��Ԃ����Z�b�g
**/
	void Timer_reset(Timer *this_Timer);

#endif

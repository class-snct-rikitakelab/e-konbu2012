#ifndef __Timer
#define __Timer


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
	void Timer_detcPassTargTimer(Timer *this_Timer);

#endif

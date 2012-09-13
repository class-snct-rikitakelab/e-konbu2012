#ifndef __Timer
#define __Timer


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
	void Timer_detcPassTargTimer(Timer *this_Timer);

#endif

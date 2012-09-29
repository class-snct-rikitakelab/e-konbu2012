
#include <string>
#include <vector>
#include <list>
#include <iostream>
#include <assert.h>

namespace DetectionPart
{
/**
 * ŽžŠÔŒv
 */
class Timer
{
private:
	int targTimer;


public:
	void Timer_init(Timer *this_Timer);

	/**
	 * Žw’èŽžŠÔ‚ðŒŸ’m‚·‚é
	 */
	void Timer_detcPassTargTimer(Timer *this_Timer);

};

}  // namespace DetectionPart

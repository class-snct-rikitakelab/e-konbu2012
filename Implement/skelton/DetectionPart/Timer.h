
#include <string>
#include <vector>
#include <list>
#include <iostream>
#include <assert.h>

namespace DetectionPart
{
/**
 * ���Ԍv
 */
class Timer
{
private:
	int targTimer;


public:
	void Timer_init(Timer *this_Timer);

	/**
	 * �w�莞�Ԃ����m����
	 */
	void Timer_detcPassTargTimer(Timer *this_Timer);

};

}  // namespace DetectionPart


#include <string>
#include <vector>
#include <list>
#include <iostream>
#include <assert.h>

namespace DetectionPart
{
/**
 * 時間計
 */
class Timer
{
private:
	int targTimer;


public:
	/**
	 * 指定時間を検知する
	 */
	void Timer_detcPassTargTimer(Timer *this_Timer);

};

}  // namespace DetectionPart

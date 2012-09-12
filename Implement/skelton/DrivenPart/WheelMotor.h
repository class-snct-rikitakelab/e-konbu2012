
#include <string>
#include <vector>
#include <list>
#include <iostream>
#include <assert.h>

#include "DrivenPart/S8.h"

namespace DrivenPart
{
/**
 * 車輪サーボモータ
 */
class WheelMotor
{
public:
	/**
	 * モータを駆動する
	 */
	void WheelMotor_driveMotor(WheelMotor *this_WheelMotor, S8 motorCtrlVal);

	void WheelMotor_init(WheelMotor *this_WheelMotor);

};

}  // namespace DrivenPart

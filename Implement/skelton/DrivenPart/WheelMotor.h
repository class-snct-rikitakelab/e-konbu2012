
#include <string>
#include <vector>
#include <list>
#include <iostream>
#include <assert.h>

#include "DrivenPart/S8.h"

namespace DrivenPart
{
/**
 * �ԗփT�[�{���[�^
 */
class WheelMotor
{
public:
	/**
	 * ���[�^���쓮����
	 */
	void WheelMotor_driveMotor(WheelMotor *this_WheelMotor, S8 motorCtrlVal);

	void WheelMotor_init(WheelMotor *this_WheelMotor);

};

}  // namespace DrivenPart

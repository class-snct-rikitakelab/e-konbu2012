#ifndef __WheelMotor
#define __WheelMotor

#include "ecrobot_interface.h"

/**
 * �ԗփT�[�{���[�^
 */
typedef struct {
	U32 PORT;
}WheelMotor;

	/**
	 * ���[�^���쓮����
	 */
	void WheelMotor_driveMotor(WheelMotor *this_WheelMotor, S8 motorCtrlVal);

	void WheelMotor_init(WheelMotor *this_WheelMotor,U32 port);

#endif


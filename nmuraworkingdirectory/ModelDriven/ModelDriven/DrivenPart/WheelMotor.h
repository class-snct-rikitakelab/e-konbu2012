#ifndef __WheelMotor
#define __WheelMotor

#include "ecrobot_interface.h"

/**
 * 車輪サーボモータ
 */
typedef struct {
	U32 PORT;
}WheelMotor;

	/**
	 * モータを駆動する
	 */
	void WheelMotor_driveMotor(WheelMotor *this_WheelMotor, S8 motorCtrlVal);

	void WheelMotor_init(WheelMotor *this_WheelMotor,U32 port);

#endif


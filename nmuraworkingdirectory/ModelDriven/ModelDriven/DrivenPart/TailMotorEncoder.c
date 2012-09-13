#include "TailMotorEncoder.h"

void TailMotorEncoder_init(TailMotorEncoder *this_TailMotorEncoder)
{
}

S32 TailMotorEncoder_getTailAngle(TailMotorEncoder *this_TailMotorEncoder)
{
	return ecrobot_get_motor_rev(NXT_PORT_A);
}

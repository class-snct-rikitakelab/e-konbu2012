#include "TailMotor.h"

void TailMotor_init(TailMotor *this_TailMotor)
{
	ecrobot_set_motor_speed(NXT_PORT_A,0);
	ecrobot_set_motor_rev(NXT_PORT_A,0);
}

void TailMotor_setTailSpeed(TailMotor *this_TailMotor, S8 parm)
{
	ecrobot_set_motor_speed(NXT_PORT_A,parm);
}

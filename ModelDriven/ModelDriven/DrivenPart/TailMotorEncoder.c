#include "TailMotorEncoder.h"

/*
 *	TailMotorEncoder.c
 *	�K���p�x�v����N���X
 */

//���������\�b�h
void TailMotorEncoder_init(TailMotorEncoder *this_TailMotorEncoder)
{
	/*
	ecrobot_set_motor_speed(NXT_PORT_A,0);
	ecrobot_set_motor_rev(NXT_PORT_A,0);
	*/
}

//�v�����\�b�h
int TailMotorEncoder_getTailAngle(TailMotorEncoder *this_TailMotorEncoder)
{
	return nxt_motor_get_count(NXT_PORT_A);
}

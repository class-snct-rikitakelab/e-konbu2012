#include "TailMotor.h"
#include "../Common/Factory.h"
/*
 *	TailMotor.c
 *	�K���T�[�{���[�^�N���X
 */

//�������N���X
void TailMotor_init(TailMotor *this_TailMotor)
{
	/*
	ecrobot_set_motor_speed(NXT_PORT_A,0);
	ecrobot_set_motor_rev(NXT_PORT_A,0);
	*/
}

//���상�\�b�h
void TailMotor_setTailSpeed(TailMotor *this_TailMotor, int parm)
{
	nxt_motor_set_speed(NXT_PORT_A,parm,1);
}

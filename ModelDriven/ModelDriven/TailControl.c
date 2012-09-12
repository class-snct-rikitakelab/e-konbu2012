/*
 *	TailControl.c
 *	K”öŠp“x§ŒäƒNƒ‰ƒX
 */

#include "TailControl.h"

void TailControl_init(TailControl * this_TailControl)
{
	this_TailControl -> Tail_Kp = 2.4;
	this_TailControl -> Tail_Ki = 0;
	this_TailControl -> Tail_Kd = 0;
	this_TailControl -> State = TAIL_UP;
}

//K”öŠp“xPID§Œäƒƒ\ƒbƒh
void TailControl_PIDTailControl(TailControl * this_TailControl)
{
	static float Tail_hensa = 0;
	static float Tail_i_hensa = 0;
	static float Tail_d_hensa = 0;
	static float Tail_bf_hensa = 0;
	static float Tail_speed = 0;

	switch(this_TailControl -> State){
		case (TAIL_DOWN):
			Tail_hensa = ANGLEOFDOWN - ecrobot_get_motor_rev(NXT_PORT_A);
			break;
		case(TAIL_UP):
			Tail_hensa = ANGLEOFUP - ecrobot_get_motor_rev(NXT_PORT_A);
			break;
		default:
			break;
	}

	Tail_i_hensa = Tail_i_hensa + (Tail_hensa * 0.002);
	Tail_d_hensa = (Tail_hensa - Tail_bf_hensa)/0.002;
	Tail_bf_hensa = Tail_hensa;

	Tail_speed = this_TailControl -> Tail_Kp * Tail_hensa + this_TailControl -> Tail_Ki * Tail_i_hensa	
		+ this_TailControl -> Tail_Kd * Tail_d_hensa;

	if (Tail_speed < -100)
		Tail_speed = -100;
	else if (Tail_speed > 100)
		Tail_speed = 100;

	ecrobot_set_motor_speed(NXT_PORT_A,Tail_speed);
}

//–Ú•WK”öŠp“x•ÏXƒƒ\ƒbƒh
void TailControl_TailStateChange(TailControl * this_TailControl,TailState state)
{
	this_TailControl -> State = state;
}

#include "LightPIDControl.h"

void RA_linetrace_PID(int forward_speed) {

	//PID����p�΍��l
	static float hensa;					//P����p
	static float i_hensa = 0;			//I����p
	static float d_hensa = 0;			//D����p
	static float bf_hensa = 0;
	float right_motor_turn = 0,left_motor_turn = 0;					//�ڕWpwm�l
	float right_motor_turn_overflow = 0,left_motor_turn_overflow = 0;		//�ڕWpwm�l�I�[�o�[�t���[��

	
	forward_speed = RA_speed(forward_speed);	//���x��i�K�I�ɕω�

	//���Z���T�l�ƖڕW�P�x�l�̕΍��Z�o
	if(forward_speed > 0)
		hensa = (float)GRAY_VALUE - (float)ecrobot_get_light_sensor(NXT_PORT_S3);
	else
		hensa = (float)ecrobot_get_light_sensor(NXT_PORT_S3) - (float)GRAY_VALUE;

	i_hensa = i_hensa+(hensa*0.0002);		//0��1����
	d_hensa = (hensa - bf_hensa)/0.0002;
	bf_hensa = hensa;

	cmd_turn = -(Kp * hensa + Ki * i_hensa + Kd * d_hensa);

	right_motor_turn = forward_speed - cmd_turn/2;
	left_motor_turn = forward_speed  + cmd_turn/2;


	//�I�[�o�[�t���[�΍�y�уI�[�o�[�t���[���l��
	if (/*-128*/-100 > right_motor_turn) {
		right_motor_turn_overflow = -(-128 - right_motor_turn);
		right_motor_turn = -128;
	} else if (100/*127*/ < right_motor_turn) {
		right_motor_turn_overflow = right_motor_turn - 100/*-127*/;
		right_motor_turn = 127;
	}

	if (/*-128*/-100 > left_motor_turn) {
		left_motor_turn_overflow = -(/*-128*/-100 - left_motor_turn);
		left_motor_turn = -128;
	} else if (/*127*/100 < left_motor_turn) {
		left_motor_turn_overflow = left_motor_turn -100/*- 127*/;
		left_motor_turn = 100/*127*/;
	}

	//�o��pwm�l�Z�o
	if(left_motor_turn + right_motor_turn_overflow >= -100/*-128*/ && left_motor_turn + right_motor_turn_overflow <= 100/*127*/)
		pwm_l = (int)(left_motor_turn + right_motor_turn_overflow);
	else
		pwm_l = (int)left_motor_turn;

	if(right_motor_turn + left_motor_turn_overflow >= -100/*-128*/ && right_motor_turn + left_motor_turn_overflow <= 100/*127*/)
		pwm_r = (int)(right_motor_turn + left_motor_turn_overflow);
	else
		pwm_r = (int)right_motor_turn;

}
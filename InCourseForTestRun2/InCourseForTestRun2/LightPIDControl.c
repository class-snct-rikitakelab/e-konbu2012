#include "LightPIDControl.h"

/*
 *	輝度値PIDライントレースメソッド
 *	引数:目標速度（RA_speed(速度)にすること）
 *	返り値:旋回量
 */

int RA_linetrace_PID(int forward_speed) {

	int cmd_turn;
	/*
	float right_motor_turn = 0,left_motor_turn = 0;					//目標pwm値
	float right_motor_turn_overflow = 0,left_motor_turn_overflow = 0;		//目標pwm値オーバーフロー分
	*/
	//光センサ値と目標輝度値の偏差算出
	if(forward_speed > 0)
		hensa = (float)GRAY_VALUE - (float)ecrobot_get_light_sensor(NXT_PORT_S3);
	else
		hensa = (float)ecrobot_get_light_sensor(NXT_PORT_S3) - (float)GRAY_VALUE;

	i_hensa = i_hensa+(hensa*0.0002);		//0が1つ多い
	d_hensa = (hensa - bf_hensa)/0.0002;
	bf_hensa = hensa;

	cmd_turn = -(Kp * hensa + Ki * i_hensa + Kd * d_hensa);
	/*
	right_motor_turn = forward_speed - cmd_turn/2;
	left_motor_turn = forward_speed  + cmd_turn/2;


	//オーバーフロー対策及びオーバーフロー分考慮
	if (-100 > right_motor_turn) {
		right_motor_turn_overflow = -(-128 - right_motor_turn);
		right_motor_turn = -128;
	} else if (100 < right_motor_turn) {
		right_motor_turn_overflow = right_motor_turn - 100;
		right_motor_turn = 127;
	}

	if (-100 > left_motor_turn) {
		left_motor_turn_overflow = -(-100 - left_motor_turn);
		left_motor_turn = -128;
	} else if (100 < left_motor_turn) {
		left_motor_turn_overflow = left_motor_turn -100;
		left_motor_turn = 100;
	}

	//出力pwm値算出
	if(left_motor_turn + right_motor_turn_overflow >= -100 && left_motor_turn + right_motor_turn_overflow <= 100)
		pwm_l = (int)(left_motor_turn + right_motor_turn_overflow);
	else
		pwm_l = (int)left_motor_turn;

	if(right_motor_turn + left_motor_turn_overflow >= -100 && right_motor_turn + left_motor_turn_overflow <= 100)
		pwm_r = (int)(right_motor_turn + left_motor_turn_overflow);
	else
		pwm_r = (int)right_motor_turn;
	*/
	return cmd_turn;
}

//ON-OFF制御ライントレース関数
int RA_linetrace() {
	int turn_speed = 30;
	int cmd_turn;

	int light_value = 0;

	light_value = online();

	if (TRUE != light_value) {
		cmd_turn = turn_speed;
	} else {
		cmd_turn = (-1)*turn_speed;
	}

	return cmd_turn;
}

/*
 *	輝度値PID制御の偏差値リセットメソッド
 *	主にしっぽ走行から倒立走行に切り替える際に使用
 *	引数:無し
 *	返り値:
 */
void RA_hensareset(void)
{
	hensa = 0;
	i_hensa = 0;
	d_hensa = 0;
	bf_hensa = 0;
}

//ON-OFF制御用ライン判定関数
int online(void) {

	int light_value;
	light_value = ecrobot_get_light_sensor(NXT_PORT_S3);	//現在の輝度値
					
	if (LOOKUP_GRAY_VALUE > light_value)		//輝度値が目標値より大きいか判断
		return FALSE;					//ライン外
	else
		return TRUE;					//ライン内

}
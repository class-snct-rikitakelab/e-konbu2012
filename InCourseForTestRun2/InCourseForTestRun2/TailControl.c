#include "TailControl.h"

//尻尾角度調節関数

void taildown(){

	//X-ecrobot_get_motor_rev(NXT_PORT_S4) のX = 目標値
	//目標値を1ずつ調節(現行目標値t_angleそのものを最終目標値t_valueへ近づけていく)
	//t_angleの初期値は現在の角度

	static float t_hensa = 0;	//尻尾角度の目標値との差
	static float t_ihensa = 0;	//I制御用偏差

	static float t_speed = 0;	//尻尾モータに送る速度

	t_count++;					//速度制御用カウンタ


	/*	一つの状態でできないかどうか思案中
	if(ecrobot_get_motor_rev(NXT_PORT_A) < t_value)
	{
		t_hensa = t_angle - ecrobot_get_motor_rev(NXT_PORT_A);
		if(t_count >= t_count_limit)	//カウンタで尻尾を下げる速度を調整
		{
			t_angle+=t_up;			//角度を上げる
			t_count = 0;
		}
	}

	else if(ecrobot_get_motor_rev(NXT_PORT_A) > t_value)
	{
		t_hensa = t_angle - ecrobot_get_motor_rev(NXT_PORT_A);
		if(t_count >= t_count_limit)	//カウンタで尻尾を上げる速度を調整
		{
			t_angle-=t_up;			//角度を下げる
			t_count = 0;
		}
	}

	else
	{
		t_hensa = t_value - ecrobot_get_motor_rev(NXT_PORT_A);
	}
	
	
	
	switch(tail_mode){
		case(RN_TAILDOWN):				//尻尾を下げる
			if(ecrobot_get_motor_rev(NXT_PORT_A) < t_value)		//現在の角度が目標値以下かどうか
			{
				t_hensa = t_angle - ecrobot_get_motor_rev(NXT_PORT_A);
				if(t_count >= t_count_limit)	//カウンタで尻尾を下げる速度を調整
				{
					t_angle += t_up;			//角度を上げる
					t_count = 0;
				}
			}
			else// if(ecrobot_get_motor_rev(NXT_PORT_A) == t_value)
			{
//				t_angle = t_value;
				t_hensa = t_value - ecrobot_get_motor_rev(NXT_PORT_A);
			}
			
			break;

		case(RN_TAILUP):										//尻尾を上げる
			if(ecrobot_get_motor_rev(NXT_PORT_A) > t_value)		//現在の角度が目標値以上かどうか
			{
				t_hensa = t_angle - ecrobot_get_motor_rev(NXT_PORT_A);
				if(t_count >= t_count_limit)					//カウンタで尻尾を上げる速度を調整
				{
					t_angle -= t_up;							//角度を下げる
					t_count = 0;
				}
			}
			else
			{
//				t_angle = t_value;
				t_hensa = t_value - ecrobot_get_motor_rev(NXT_PORT_A);
			}
			
			break;

		default:
			break;
			
	}
	*/

	switch(tail_mode){
		case(RN_TAILDOWN):				//尻尾を下げる
			if(t_angle <= t_value)		//現在の角度が目標値以下かどうか
			{
				t_hensa = t_angle - ecrobot_get_motor_rev(NXT_PORT_A);
				if(t_count >= t_count_limit)	//カウンタで尻尾を下げる速度を調整
				{
					t_angle+=t_up;			//角度を上げる
					t_count = 0;
				}
			}
			else
			{
				t_angle = t_value;
			}

			break;

		case(RN_TAILUP):				//尻尾を上げる
			if(t_angle >= t_value)		//現在の角度が目標値以上かどうか
			{
				t_hensa = t_angle - ecrobot_get_motor_rev(NXT_PORT_A);
				if(t_count >= t_count_limit)	//カウンタで尻尾を上げる速度を調整
				{
					t_angle-=t_up;			//角度を下げる
					t_count = 0;
				}
			}
			else
			{
				t_angle = t_value;
			}

			break;

		default:
			break;

	}

	t_ihensa = t_ihensa+(t_hensa*0.0004);		//I制御用偏差

	t_speed = (t_Kp*t_hensa + t_Ki*t_ihensa);	//偏差を元に速度調節

	if (t_speed < -100)
		t_speed = -100;
	else if (t_speed > 100)
		t_speed = 100;

	ecrobot_set_motor_speed(NXT_PORT_A, t_speed);	//モータに速度を送る

}

//尻尾モード変更関数
void tail_mode_change(int mode,int value,int limit,int up)	//mode(0:尻尾を下ろす、1:尻尾を上げる) value(目標値) limit(上げるほど遅延） up（上げるほど加速）1
{

	static int flag;					//尻尾モードフラグ

	switch(mode){
		case 0:
			tail_mode = RN_TAILDOWN;	//尻尾を下ろす
			flag = 0;
			break;
		case 1:
			tail_mode = RN_TAILUP;		//尻尾を上げる
			flag = 1;
			break;
		default:
			break;
	}

	//目標値が変わった時のみ、角度を決める（複数回呼び出す場合も初期値をキープするため）
	if(t_value != value)
		t_angle = ecrobot_get_motor_rev(NXT_PORT_A);

	t_value = value;			//目標値設定
	t_count_limit = limit;		//カウンタ最大値設定
	t_up = up;					//角度増減値設定

}
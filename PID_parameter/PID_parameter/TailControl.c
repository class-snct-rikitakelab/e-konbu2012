#include "TailControl.h"

//尻尾角度調節関数

/*
 *	しっぽモータ制御及び出力メソッド
 *	一定周期で呼び出すことでしっぽを制御
 *	目標角度はcontroltailmodeに依存
 */
void TailControl(){
	
	TargetTailAngleControl();

	static float t_hensa = 0;
	static float t_speed = 0;

	t_hensa = result_angle - ecrobot_get_motor_rev(NXT_PORT_A);		//尻尾を下げる

	t_speed = t_Kp*t_hensa;

	if (t_speed < -100)
		t_speed = -100;
	else if (t_speed > 100)
		t_speed = 100;

	ecrobot_set_motor_speed(NXT_PORT_A, t_speed);
}


/*
 *	しっぽモータ角度制御用目標値制御メソッド
 *	制御目標値を徐々に調節
 *	TAILUPにする場合のみ、制御を行わない
 */
void TargetTailAngleControl(){
	static int counter = 0;

	int target_angle;

	switch(controltailmode){
	case (RN_TAILDOWN):
		target_angle = ANGLEOFDOWN;
		break;
	case (RN_TAILUP):
		target_angle = ANGLEOFUP;
		result_angle = ANGLEOFUP;
		break;
	case (RN_TAILLOOKUP):
		target_angle = ANGLEOFLOOKUP;
		break;
	case (RN_TAILSTAND):
		target_angle = ANGLEOFSTAND;
		break;
	case (RN_TAILPUSH):
		target_angle = ANGLEOFPUSH;
		break;
	default:
		break;
	}

	counter++;

	if(counter >= TAIL_ANGLE_COUNT)
	{
		if(target_angle - result_angle >= 0){
			result_angle++;

			if(result_angle > target_angle)
				result_angle = target_angle;
		}
		else{
			result_angle--;

			if(result_angle < target_angle)
				result_angle = target_angle;
		}
		counter = 0;
	}
}

/*
 *	しっぽモータ目標角度設定関数
 *	外部からしっぽの角度を変えたい時に呼び出す
 */
void TailModeChange(RN_TAILMODE tail_mode){
	controltailmode = tail_mode;	
}

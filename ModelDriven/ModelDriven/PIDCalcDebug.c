/*
 *	PIDCalcDebug.c
 *	PID制御クラス
 */

#include "PIDCalcDebug.h"

#include "Factory.h"

void PIDCalcDebug_init(PIDCalcDebug * this_PIDCalcDebug)
{
	this_PIDCalcDebug -> Kp = 0.436;		//P制御用
	this_PIDCalcDebug -> Ki = 1.31;			//I制御用
	this_PIDCalcDebug -> Kd = 0.011;		//D制御用
}

//旋回量輝度値PID制御実行メソッド
float PIDCalcDebug_PIDLinetrace(PIDCalcDebug * this_PIDCalcDebug)
{
	//PID制御用偏差値
	static float hensa;					//P制御用
	static float i_hensa = 0;			//I制御用
	static float d_hensa = 0;			//D制御用
	static float bf_hensa = 0;
	float turn = 0;						//旋回量

	hensa = (float)Calibration_getGrayValue(&mCalibration) - (float)ecrobot_get_light_sensor(NXT_PORT_S3);	//偏差計算

	i_hensa = i_hensa+(hensa*0.002);
	d_hensa = (hensa - bf_hensa)/0.002;
	bf_hensa = hensa;

	turn = -(this_PIDCalcDebug -> Kp * hensa + this_PIDCalcDebug -> Ki * i_hensa	//旋回量計算
		+ this_PIDCalcDebug -> Kd * d_hensa);

	return turn;
}

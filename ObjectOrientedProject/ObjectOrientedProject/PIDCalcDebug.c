/*
 *	PIDCalcDebug.c
 *	PID����N���X
 */

#include "PIDCalcDebug.h"

#include "Factory.h"

void PIDCalcDebug_init(PIDCalcDebug * this_PIDCalcDebug)
{
	this_PIDCalcDebug -> Kp = 0.436;		//P����p
	this_PIDCalcDebug -> Ki = 1.31;			//I����p
	this_PIDCalcDebug -> Kd = 0.011;		//D����p
}

//����ʋP�x�lPID������s���\�b�h
float PIDCalcDebug_PIDLinetrace(PIDCalcDebug * this_PIDCalcDebug)
{
	//PID����p�΍��l
	static float hensa;					//P����p
	static float i_hensa = 0;			//I����p
	static float d_hensa = 0;			//D����p
	static float bf_hensa = 0;
	float turn = 0;						//�����

	hensa = (float)Calibration_getGrayValue(&mCalibration) - (float)ecrobot_get_light_sensor(NXT_PORT_S3);	//�΍��v�Z

	i_hensa = i_hensa+(hensa*0.002);
	d_hensa = (hensa - bf_hensa)/0.002;
	bf_hensa = hensa;

	turn = -(this_PIDCalcDebug -> Kp * hensa + this_PIDCalcDebug -> Ki * i_hensa	//����ʌv�Z
		+ this_PIDCalcDebug -> Kd * d_hensa);

	return turn;
}
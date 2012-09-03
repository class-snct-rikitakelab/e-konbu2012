/*
 *	Calibration.c
 *	キャリブレーション実行クラス
 */


#include "Calibration.h"
#include "Factory.h"


void Calibration_init(Calibration * this_Calibration)
{
	this_Calibration ->blackValue = 0;
	this_Calibration ->whiteValue = 0;
	this_Calibration ->grayValue = 0;
	this_Calibration ->State = GET_BLACK;
}

//キャリブレーション実行メソッド
int Calibration_doCalibrate(Calibration * this_Calibration)
{
	int calibrationFlag = 0;

	//黒値取得
	while(1){
		if(ecrobot_get_touch_sensor(NXT_PORT_S4) == TRUE)
		{
			ecrobot_sound_tone(880, 512, 10);
			this_Calibration -> blackValue=ecrobot_get_light_sensor(NXT_PORT_S3);
			systick_wait_ms(500);
			break;
		}
	}

	//白値取得
	while(1){
		if(ecrobot_get_touch_sensor(NXT_PORT_S4) == TRUE)
		{
			ecrobot_sound_tone(906, 512, 10);
			this_Calibration -> whiteValue=ecrobot_get_light_sensor(NXT_PORT_S3);
			systick_wait_ms(500);
			break;
		}
	}

	//灰色値計算
	this_Calibration -> grayValue=(this_Calibration -> blackValue + this_Calibration -> whiteValue)/2;

	//ジャイロオフセット取得
	while(1){
		if(ecrobot_get_touch_sensor(NXT_PORT_S4) == TRUE)
		{
			ecrobot_sound_tone(932, 512, 10);
			PWMValGenerator_setGyroOffset(&mPWMValGenerator,(U32)ecrobot_get_gyro_sensor(NXT_PORT_S1));
			systick_wait_ms(500);
			calibrationFlag = 1;
			break;
		}
	}

	return calibrationFlag;
}

//灰色値取得メソッド
int Calibration_getGrayValue(Calibration * this_Calibration)
{
	return this_Calibration ->blackValue;
}

#include "Calibration.h"

//キャリブレーション関数
int RN_calibrate(void)
{
	int calibrationendflag = 0;

	TailAngleChange(ANGLEOFDOWN);

	//黒値
	while(1){
		if(ecrobot_get_touch_sensor(NXT_PORT_S4) == TRUE)
		{
			ecrobot_sound_tone(880, 512, 10);
			BLACK_VALUE=ecrobot_get_light_sensor(NXT_PORT_S3);
			systick_wait_ms(500);
			break;
		}
	}

	//白値
	while(1){
		if(ecrobot_get_touch_sensor(NXT_PORT_S4) == TRUE)
		{
			ecrobot_sound_tone(906, 512, 10);
			WHITE_VALUE=ecrobot_get_light_sensor(NXT_PORT_S3);
			systick_wait_ms(500);
			break;
		}
	}

	//灰色値計算
	GRAY_VALUE=(BLACK_VALUE+WHITE_VALUE)/2;

	//尻尾をルックアップゲート時の角度に
	TailAngleChange(ANGLEOFLOOKUP);

	//ルックアップゲート用黒値
	while(1){
		if(ecrobot_get_touch_sensor(NXT_PORT_S4) == TRUE)
		{
			ecrobot_sound_tone(880, 512, 10);
			LOOKUP_BLACK_VALUE=ecrobot_get_light_sensor(NXT_PORT_S3);
			systick_wait_ms(500);
			break;
		}
	}

	//ルックアップゲート用白値
	while(1){
		if(ecrobot_get_touch_sensor(NXT_PORT_S4) == TRUE)
		{
			ecrobot_sound_tone(906, 512, 10);
			LOOKUP_WHITE_VALUE=ecrobot_get_light_sensor(NXT_PORT_S3);
			systick_wait_ms(500);
			break;
		}
	}

	//ルックアップゲート用灰色値計算
	LOOKUP_GRAY_VALUE=(LOOKUP_BLACK_VALUE+LOOKUP_WHITE_VALUE)/2;

	//尻尾を直立停止状態の角度に
	TailAngleChange(ANGLEOFDOWN);

	//ジャイロオフセット及びバッテリ電圧値
	while(1){
		if(ecrobot_get_touch_sensor(NXT_PORT_S4) == TRUE)
		{
			ecrobot_sound_tone(932, 512, 10);
			setGyroOffset((U32)ecrobot_get_gyro_sensor(NXT_PORT_S1));
			INIT_GYRO_OFFSET = gyro_offset;
			systick_wait_ms(500);
			break;
		}
	}

	//走行開始合図
	while(1)
	{
		//リモートスタート
		if(remote_start()==1)
		{
			ecrobot_sound_tone(982,512,30);
			calibrationendflag = 1;
			break;
		}

		//タッチセンサスタート
		else if (ecrobot_get_touch_sensor(NXT_PORT_S4) == TRUE)
		{
			ecrobot_sound_tone(982,512,10);
			while(1)
			{
					if (ecrobot_get_touch_sensor(NXT_PORT_S4) != TRUE)
					{
						calibrationendflag = 1;
						break;
					}
			}
			break;
		}
	}
	
	return calibrationendflag;

}

U32 getGyroOffset()
{
	return gyro_offset;
}

void setGyroOffset(int setvalue)
{
	gyro_offset = setvalue;
}

U32 getGyroOffsetInit()
{
	return INIT_GYRO_OFFSET;
}

unsigned int getGrayValue()
{
	return GRAY_VALUE;
}

unsigned int getLookUpGrayValue()
{
	return LOOKUP_GRAY_VALUE;
}
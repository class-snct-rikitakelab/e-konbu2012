
#include "kernel.h"
#include "kernel_id.h"
#include "ecrobot_interface.h"
#include "balancer.h" /* 倒立振子制御用ヘッダファイル */
#include "GyroVariation.h"
#include "logSend.h"




void GyroVariation_init(GyroVariation * this_GyroVariation){
	this_GyroVariation->gyroSensorVarieation = 0;
}

void GyroVariation_calGyroSensorVariation(GyroVariation * this_GyroVariation)
{
	static int executeOnceFlag=0;
	static	U16 bufGyroSensorVal;
	
	//一回だけ行いたい処理
	if(executeOnceFlag==0){
		bufGyroSensorVal = (U16)ecrobot_get_gyro_sensor(NXT_PORT_S3); //初回のみバッファに最新の値を保存
		executeOnceFlag=1;
	}

	this_GyroVariation->gyroSensorVarieation = (S16)ecrobot_get_gyro_sensor(NXT_PORT_S3) -(S16) bufGyroSensorVal; //変化量の計算
	bufGyroSensorVal = (U16)ecrobot_get_gyro_sensor(NXT_PORT_S3); //一回前の変化量の保存
	/*
	logSend(0,0,0,ecrobot_get_gyro_sensor(NXT_PORT_S1),
		0,this_GyroVariation->gyroSensorVarieation,0);
		*/
}

S16 GyroVariation_getGyroSensorVariation(GyroVariation * this_GyroVariation)
{
	return this_GyroVariation->gyroSensorVarieation;
}


 /*
void monitorVariation()
{
	static int counter;

	
	++counter;
if(counter==240/4){//約20ms
	
	calGyroSensorVarieation();
	
	switch (detectLineState) {
		
		case OnLine :
		//ラインアウトしたら
		if(gyroSensorVarieation < DETECTLINEOUT_THRESHOLD ) {
		setDetectLineState(OutOfLine);
		ecrobot_sound_tone(880, 512, 30);
		systick_wait_ms(500);
		}
		else if(gyroSensorVarieation > DETECTLINE_IN_THRESHOLD) {
		setDetectLineState(OnLine);
		}
		//OnLine状態なのに、光センサの値が白色付近だったら。
		else if((ecrobot_get_gyro_sensor(NXT_PORT_S3)  > getWhiteValue() -20  && ecrobot_get_gyro_sensor(NXT_PORT_S3) < getWhiteValue() +20 )) {
  		setDetectLineState(OnLine);
		ecrobot_sound_tone(440, 512, 30);
		systick_wait_ms(500);
		}
		break;

		case OutOfLine :
		//ラインに乗ったら
		if(gyroSensorVarieation > DETECTLINE_IN_THRESHOLD) {
		setDetectLineState(OnLine);
		ecrobot_sound_tone(440, 512, 30);
		systick_wait_ms(500);
		}
		else if(gyroSensorVarieation < DETECTLINEOUT_THRESHOLD ) {
		setDetectLineState(OutOfLine);
		}
		//OutOfLine状態なのに値が目標値付近だったら
		else if((ecrobot_get_gyro_sensor(NXT_PORT_S3)  > getLineMokuhyouValue() -20  && ecrobot_get_gyro_sensor(NXT_PORT_S3) < getLineMokuhyouValue() +20 )) {
  		setDetectLineState(OnLine);
		ecrobot_sound_tone(440, 512, 30);
		systick_wait_ms(500);
		}
		break;

		default : 
		//none
		break;
	}
	

	
	//ラインアウトしたら
	if(gyroSensorVarieation < DETECTLINEOUT_THRESHOLD ) {		

		switch (detectLineState) {
		
		case OnLine :
		setDetectLineState(OutOfLine);
		ecrobot_sound_tone(880, 512, 30);
		systick_wait_ms(500);
		break;
		
		case OutOfLine :
		setDetectLineState(OutOfLine);
		break;

		default : 
		//none
		break;
		}

	}
	//ラインを検知したら
	else if(gyroSensorVarieation > DETECTLINE_IN_THRESHOLD ) {		

		switch (detectLineState) {
		
		case OnLine :
		setDetectLineState(OnLine);
		break;
		
		case OutOfLine :
		setDetectLineState(OnLine);
		ecrobot_sound_tone(440, 512, 30);
		systick_wait_ms(500);
		break;

		default : 
		//none
		break;
		}

	}
	else if (ecrobot_get_gyro_sensor(NXT_PORT_S3) -20 > mokuhyouti && ecrobot_get_gyro_sensor(NXT_PORT_S3)+20 < mokuhyouti) {
		
		switch (detectLineState) {
		
		case OnLine :
		setDetectLineState(OnLine);
		break;
		
		case OutOfLine :
		setDetectLineState(OnLine);
		ecrobot_sound_tone(440, 512, 30);
		systick_wait_ms(500);
		break;

		default : 
		//none
		break;
		}


	}
	
counter = 0;
}
}
*/



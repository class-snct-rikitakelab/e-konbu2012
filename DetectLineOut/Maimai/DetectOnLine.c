
#include "kernel.h"
#include "kernel_id.h"
#include "ecrobot_interface.h"
#include "balancer.h" /* 倒立振子制御用ヘッダファイル */
#include "DetectOnLine.h"

extern unsigned int getWhiteValue();
extern unsigned int getLineMokuhyouValue();


static	float lightSensorVarieation;
static	float BufLightSensorVal;
static	DetectLineState detectLineState= OnLine;


void setDetectLineState(DetectLineState state)
{
	detectLineState= state;
}
DetectLineState getDetectLineState()
{
	return detectLineState;
}
 
void calLightSensorVarieation()
{		
	lightSensorVarieation = (float)ecrobot_get_light_sensor(NXT_PORT_S3) - BufLightSensorVal; //変化量の計算
	BufLightSensorVal = (float)ecrobot_get_light_sensor(NXT_PORT_S3); //一回前のライトセンサの変化量の保存
}
 
void monitorVariation()
{
	static int counter;

	
	++counter;
if(counter==240/4){//約20ms
	
	calLightSensorVarieation();

	switch (detectLineState) {
		
		case OnLine :
		//ラインアウトしたら
		if(lightSensorVarieation < DETECTLINEOUT_THRESHOLD ) {
		setDetectLineState(OutOfLine);
		ecrobot_sound_tone(880, 512, 30);
		systick_wait_ms(500);
		}
		else if(lightSensorVarieation > DETECTLINE_IN_THRESHOLD) {
		setDetectLineState(OnLine);
		}
		//OnLine状態なのに、光センサの値が白色付近だったら。
		else if((ecrobot_get_light_sensor(NXT_PORT_S3)  > getWhiteValue() -20  && ecrobot_get_light_sensor(NXT_PORT_S3) < getWhiteValue() +20 )) {
  		setDetectLineState(OnLine);
		ecrobot_sound_tone(440, 512, 30);
		systick_wait_ms(500);
		}
		break;

		case OutOfLine :
		//ラインに乗ったら
		if(lightSensorVarieation > DETECTLINE_IN_THRESHOLD) {
		setDetectLineState(OnLine);
		ecrobot_sound_tone(440, 512, 30);
		systick_wait_ms(500);
		}
		else if(lightSensorVarieation < DETECTLINEOUT_THRESHOLD ) {
		setDetectLineState(OutOfLine);
		}
		//OutOfLine状態なのに値が目標値付近だったら
		else if((ecrobot_get_light_sensor(NXT_PORT_S3)  > getLineMokuhyouValue() -20  && ecrobot_get_light_sensor(NXT_PORT_S3) < getLineMokuhyouValue() +20 )) {
  		setDetectLineState(OnLine);
		ecrobot_sound_tone(440, 512, 30);
		systick_wait_ms(500);
		}
		break;

		default : 
		//none
		break;
	}

	/*
	//ラインアウトしたら
	if(lightSensorVarieation < DETECTLINEOUT_THRESHOLD ) {		

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
	else if(lightSensorVarieation > DETECTLINE_IN_THRESHOLD ) {		

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
	else if (ecrobot_get_light_sensor(NXT_PORT_S3) -20 > mokuhyouti && ecrobot_get_light_sensor(NXT_PORT_S3)+20 < mokuhyouti) {
		
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
	*/
counter = 0;
}
}

float getLightSensorVarieation()
{
	return lightSensorVarieation;
}


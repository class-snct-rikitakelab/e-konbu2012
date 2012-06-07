
#include "kernel.h"
#include "kernel_id.h"
#include "ecrobot_interface.h"
#include "balancer.h" /* 倒立振子制御用ヘッダファイル */
#include "DetectOnLine.h"



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

counter = 0;
}
}

float getLightSensorVarieation()
{
	return lightSensorVarieation;
}


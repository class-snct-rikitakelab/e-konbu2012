
#include "kernel.h"
#include "kernel_id.h"
#include "ecrobot_interface.h"
#include "balancer.h" /* �|���U�q����p�w�b�_�t�@�C�� */
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
	lightSensorVarieation = (float)ecrobot_get_light_sensor(NXT_PORT_S3) - BufLightSensorVal; //�ω��ʂ̌v�Z
	BufLightSensorVal = (float)ecrobot_get_light_sensor(NXT_PORT_S3); //���O�̃��C�g�Z���T�̕ω��ʂ̕ۑ�
}
 
void monitorVariation()
{
	static int counter;

	
	++counter;
if(counter==240/4){//��20ms
	
	calLightSensorVarieation();

	switch (detectLineState) {
		
		case OnLine :
		//���C���A�E�g������
		if(lightSensorVarieation < DETECTLINEOUT_THRESHOLD ) {
		setDetectLineState(OutOfLine);
		ecrobot_sound_tone(880, 512, 30);
		systick_wait_ms(500);
		}
		else if(lightSensorVarieation > DETECTLINE_IN_THRESHOLD) {
		setDetectLineState(OnLine);
		}
		//OnLine��ԂȂ̂ɁA���Z���T�̒l�����F�t�߂�������B
		else if((ecrobot_get_light_sensor(NXT_PORT_S3)  > getWhiteValue() -20  && ecrobot_get_light_sensor(NXT_PORT_S3) < getWhiteValue() +20 )) {
  		setDetectLineState(OnLine);
		ecrobot_sound_tone(440, 512, 30);
		systick_wait_ms(500);
		}
		break;

		case OutOfLine :
		//���C���ɏ������
		if(lightSensorVarieation > DETECTLINE_IN_THRESHOLD) {
		setDetectLineState(OnLine);
		ecrobot_sound_tone(440, 512, 30);
		systick_wait_ms(500);
		}
		else if(lightSensorVarieation < DETECTLINEOUT_THRESHOLD ) {
		setDetectLineState(OutOfLine);
		}
		//OutOfLine��ԂȂ̂ɒl���ڕW�l�t�߂�������
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
	//���C���A�E�g������
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
	//���C�������m������
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


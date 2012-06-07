
#include "kernel.h"
#include "kernel_id.h"
#include "ecrobot_interface.h"
#include "balancer.h" /* �|���U�q����p�w�b�_�t�@�C�� */
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
	
		
	lightSensorVarieation = (float)ecrobot_get_light_sensor(NXT_PORT_S3) - BufLightSensorVal; //�ω��ʂ̌v�Z
	BufLightSensorVal = (float)ecrobot_get_light_sensor(NXT_PORT_S3); //���O�̃��C�g�Z���T�̕ω��ʂ̕ۑ�
}
 
void monitorVariation()
{
	static int counter;

	
	++counter;
if(counter==240/4){//��20ms
	
	calLightSensorVarieation();

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

counter = 0;
}
}

float getLightSensorVarieation()
{
	return lightSensorVarieation;
}


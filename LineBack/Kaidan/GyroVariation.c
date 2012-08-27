
#include "kernel.h"
#include "kernel_id.h"
#include "ecrobot_interface.h"
#include "balancer.h" /* �|���U�q����p�w�b�_�t�@�C�� */
#include "GyroVariation.h"





void GyroVariation_init(GyroVariation * this_GyroVariation){
	this_GyroVariation->gyroSensorVarieation = 0;
}

void GyroVariation_calGyroSensorVariation(GyroVariation * this_GyroVariation)
{
	static	float bufGyroSensorVal;

	this_GyroVariation->gyroSensorVarieation = (float)ecrobot_get_gyro_sensor(NXT_PORT_S3) - bufGyroSensorVal; //�ω��ʂ̌v�Z
	bufGyroSensorVal = (float)ecrobot_get_gyro_sensor(NXT_PORT_S3); //���O�̕ω��ʂ̕ۑ�
}

float GyroVariation_getGyroSensorVariation(GyroVariation * this_GyroVariation)
{
	return this_GyroVariation->gyroSensorVarieation;
}


 /*
void monitorVariation()
{
	static int counter;

	
	++counter;
if(counter==240/4){//��20ms
	
	calGyroSensorVarieation();
	
	switch (detectLineState) {
		
		case OnLine :
		//���C���A�E�g������
		if(gyroSensorVarieation < DETECTLINEOUT_THRESHOLD ) {
		setDetectLineState(OutOfLine);
		ecrobot_sound_tone(880, 512, 30);
		systick_wait_ms(500);
		}
		else if(gyroSensorVarieation > DETECTLINE_IN_THRESHOLD) {
		setDetectLineState(OnLine);
		}
		//OnLine��ԂȂ̂ɁA���Z���T�̒l�����F�t�߂�������B
		else if((ecrobot_get_gyro_sensor(NXT_PORT_S3)  > getWhiteValue() -20  && ecrobot_get_gyro_sensor(NXT_PORT_S3) < getWhiteValue() +20 )) {
  		setDetectLineState(OnLine);
		ecrobot_sound_tone(440, 512, 30);
		systick_wait_ms(500);
		}
		break;

		case OutOfLine :
		//���C���ɏ������
		if(gyroSensorVarieation > DETECTLINE_IN_THRESHOLD) {
		setDetectLineState(OnLine);
		ecrobot_sound_tone(440, 512, 30);
		systick_wait_ms(500);
		}
		else if(gyroSensorVarieation < DETECTLINEOUT_THRESHOLD ) {
		setDetectLineState(OutOfLine);
		}
		//OutOfLine��ԂȂ̂ɒl���ڕW�l�t�߂�������
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
	

	
	//���C���A�E�g������
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
	//���C�������m������
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



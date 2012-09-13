/*
 *	Calibration.c
 *	�L�����u���[�V�������s�N���X
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

//�L�����u���[�V�������s���\�b�h
int Calibration_doCalibrate(Calibration * this_Calibration)
{
	int calibrationFlag = 0;

	//���l�擾
	while(1){
		if(ecrobot_get_touch_sensor(NXT_PORT_S4) == TRUE)
		{
			ecrobot_sound_tone(880, 512, 10);
			this_Calibration -> blackValue=ecrobot_get_light_sensor(NXT_PORT_S3);
			systick_wait_ms(500);
			break;
		}
	}

	//���l�擾
	while(1){
		if(ecrobot_get_touch_sensor(NXT_PORT_S4) == TRUE)
		{
			ecrobot_sound_tone(906, 512, 10);
			this_Calibration -> whiteValue=ecrobot_get_light_sensor(NXT_PORT_S3);
			systick_wait_ms(500);
			break;
		}
	}

	//�D�F�l�v�Z
	this_Calibration -> grayValue=(this_Calibration -> blackValue + this_Calibration -> whiteValue)/2;

	//�W���C���I�t�Z�b�g�擾
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

//�D�F�l�擾���\�b�h
int Calibration_getGrayValue(Calibration * this_Calibration)
{
	return this_Calibration ->blackValue;
}

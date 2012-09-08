#include "Calibration.h"

//�L�����u���[�V�����֐�
int RN_calibrate(void)
{
	int calibrationendflag = 0;

	TailAngleChange(ANGLEOFDOWN);

	//���l
	while(1){
		if(ecrobot_get_touch_sensor(NXT_PORT_S4) == TRUE)
		{
			ecrobot_sound_tone(880, 512, 10);
			BLACK_VALUE=ecrobot_get_light_sensor(NXT_PORT_S3);
			systick_wait_ms(500);
			break;
		}
	}

	//���l
	while(1){
		if(ecrobot_get_touch_sensor(NXT_PORT_S4) == TRUE)
		{
			ecrobot_sound_tone(906, 512, 10);
			WHITE_VALUE=ecrobot_get_light_sensor(NXT_PORT_S3);
			systick_wait_ms(500);
			break;
		}
	}

	//�D�F�l�v�Z
	GRAY_VALUE=(BLACK_VALUE+WHITE_VALUE)/2;

	//�K�������b�N�A�b�v�Q�[�g���̊p�x��
	TailAngleChange(ANGLEOFLOOKUP);

	//���b�N�A�b�v�Q�[�g�p���l
	while(1){
		if(ecrobot_get_touch_sensor(NXT_PORT_S4) == TRUE)
		{
			ecrobot_sound_tone(880, 512, 10);
			LOOKUP_BLACK_VALUE=ecrobot_get_light_sensor(NXT_PORT_S3);
			systick_wait_ms(500);
			break;
		}
	}

	//���b�N�A�b�v�Q�[�g�p���l
	while(1){
		if(ecrobot_get_touch_sensor(NXT_PORT_S4) == TRUE)
		{
			ecrobot_sound_tone(906, 512, 10);
			LOOKUP_WHITE_VALUE=ecrobot_get_light_sensor(NXT_PORT_S3);
			systick_wait_ms(500);
			break;
		}
	}

	//���b�N�A�b�v�Q�[�g�p�D�F�l�v�Z
	LOOKUP_GRAY_VALUE=(LOOKUP_BLACK_VALUE+LOOKUP_WHITE_VALUE)/2;

	//�K���𒼗���~��Ԃ̊p�x��
	TailAngleChange(ANGLEOFDOWN);

	//�W���C���I�t�Z�b�g�y�уo�b�e���d���l
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

	//���s�J�n���}
	while(1)
	{
		//�����[�g�X�^�[�g
		if(remote_start()==1)
		{
			ecrobot_sound_tone(982,512,30);
			calibrationendflag = 1;
			break;
		}

		//�^�b�`�Z���T�X�^�[�g
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
#include "LightPIDControl.h"
#include "LightSensor.h"
#include "Factory.h"

/*
 *	�P�x�lPID���C���g���[�X���\�b�h
 *	����:�ڕW���x�iRA_speed(���x)�ɂ��邱�Ɓj
 *	�Ԃ�l:�����
 */

int RA_linetrace_PID(int forward_speed) {

	int cmd_turn;
	if(graybase == 0)  //���b�N�A�b�v�Q�[�g�p�̖ڕW�P�x�l��p���邩�A�����ۑ��s�p�̋P�x�l��p���邩�ؑ�
	{
	//���Z���T�l�ƖڕW�P�x�l�̕΍��Z�o
	if(forward_speed > 0)
		hensa = (float)getGrayValue() - (float)LightSensor_getLPFLightSensorVal(&mLightSensor);
	else
		hensa = (float)LightSensor_getLPFLightSensorVal(&mLightSensor) - (float)getGrayValue();
	}
	
	else
	{
		//���Z���T�l�ƖڕW�P�x�l�̕΍��Z�o
	if(forward_speed > 0)
		hensa = (float)getLookUpGrayValue() - (float)ecrobot_get_light_sensor(NXT_PORT_S3);
	else
		hensa = (float)ecrobot_get_light_sensor(NXT_PORT_S3) - (float)getLookUpGrayValue();
	}
	

	i_hensa = i_hensa+(hensa*0.004);
	d_hensa = (hensa - bf_hensa)/0.004;
	bf_hensa = hensa;

	cmd_turn = -(Kp * hensa + Ki * i_hensa + Kd * d_hensa);

	return cmd_turn;
}

//ON-OFF���䃉�C���g���[�X�֐�
int RA_linetrace() {
	int turn_speed = 30;
	int cmd_turn;

	int light_value = 0;

	light_value = online();

	if (TRUE != light_value) {
		cmd_turn = turn_speed;
	} else {
		cmd_turn = (-1)*turn_speed;
	}

	return cmd_turn;
}

/*
 *	�P�x�lPID����̕΍��l���Z�b�g���\�b�h
 *	��ɂ����ۑ��s����|�����s�ɐ؂�ւ���ۂɎg�p
 *	����:����
 *	�Ԃ�l:
 */

void RA_hensareset(void)
{
	hensa = 0;
	i_hensa = 0;
	d_hensa = 0;
	bf_hensa = 0;
}

//ON-OFF����p���C������֐�
int online(void) {

	int light_value;
	light_value = ecrobot_get_light_sensor(NXT_PORT_S3);	//���݂̋P�x�l
					
	if (LOOKUP_GRAY_VALUE > light_value)		//�P�x�l���ڕW�l���傫�������f
		return FALSE;					//���C���O
	else
		return TRUE;					//���C����

}
//���b�N�A�b�v�Q�[�g�p�̖ڕW�P�x�l��p���邩�A�����ۑ��s�p�̋P�x�l��p���邩�ؑ�
void changeGray(){
	if(graybase == 0)
		graybase = 1;
	else if(graybase == 1)
		graybase = 0;
}
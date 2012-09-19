/*
 * runner.c
 * �x�[�V�b�N�R�[�X���s�v���O����
 */


#include "runnerForTestRun.h"
#include "mymath.h"
#include "SelfLocation.h"
#include "Calibration.h"

//�ȗ����aPID����֐�
S8 RA_curvatureCtrl_PID(float target) {
	static float bf_dev = 0.0;

	float dev = R - target;
	//float i_dev = i_dev + (dev * 0.0005);'
	//float d_dev = (dev - bf_dev) / 0.0005;
	bf_dev = dev;

	//S8 turn = Kp * dev + Ki * i_dev + Kd * d_dev;
	S8 turn = 0.6 * dev;
	if (-100 > turn) {
		turn = -100;
	}
	else if (100 < turn) {
		turn = 100;
	}

	return turn;
}

//�ԑ̕���PID����֐�
S8 RA_directionCtrl_PID(float target) {
	static float bf_dev = 0.0;
	
	float dev = getTheta( ) - target;
	//float i_dev = i_dev + (dev * 0.0005);
	float d_dev = (dev - bf_dev) / 0.0005;
	bf_dev = dev;

	//S8 turn = Kp * dev + Ki * i_dev + Kd * d_dev;
	S8 turn = 1.0 * dev + 0.5 * d_dev;
	if (-100 > turn) {
		turn = -100;
	}
	else if (100 < turn) {
		turn = 100;
	}

	return turn;
}

/* ______________�A�E�g�R�[�X���s��Ԍ��o_______________________ */
void setSection_out(){
	
/* �A�E�g�R�[�X���s��� */


	static int wait_count = 0;
	wait_count++;

	float def_x  =  getXCoo() - buf_x;
	float def_y  =  getYCoo() - buf_y;
	float def_l  = getDistance() - buf_l;
	float def_th = getTheta( ) - buf_th;

	switch(crt_sect){
	case (START):			//�X�^�[�g���⓹
		if(getInitGyroOffset() - 30 > (U32)ecrobot_get_gyro_sensor(NXT_PORT_S1) && wait_count > 500){
			ecrobot_sound_tone(220, 100, 50);
			changeSection(&buf_x, &buf_y, &buf_l, &buf_th);
			crt_sect = UP_SLOPE;
			trgt_speed = trgt_speed;
		}
		trgt_R = 0.0;
		break;
	case (UP_SLOPE):		//�⓹�n�_�����_
		if(def_l >= 30 && getInitGyroOffset() - 30 > (U32)ecrobot_get_gyro_sensor(NXT_PORT_S1)){
			ecrobot_sound_tone(233, 100, 50);
			changeSection(&buf_x, &buf_y, &buf_l, &buf_th);
			crt_sect = DOWN_SLOPE;
			trgt_speed = trgt_speed -10;
		}
		trgt_R = 0.0;
		break;
	case (DOWN_SLOPE):		//���_���⓹�I�_
		//if(getDistance() >= 390){
		if(def_l >= 90){
			ecrobot_sound_tone(246, 100, 50);
			changeSection(&buf_x, &buf_y, &buf_l, &buf_th);
			crt_sect = FST_CORNER;
			trgt_speed = trgt_speed - 10;
		}
		trgt_R = 0.0;
		break;
	case (FST_CORNER):		//�⓹�I�_�����J�[�u
		if(def_x >= 74 && def_y >= 70 && def_l >= 110 && def_th >= 90){
			ecrobot_sound_tone(261, 100, 50);
			changeSection(&buf_x, &buf_y, &buf_l, &buf_th);
			crt_sect = FST_STRAIGHT;
			trgt_speed = trgt_speed +20;
		}
		
		trgt_R = 67.59;
		break;
	case (FST_STRAIGHT):	//���J�[�u�I�_�����X�g���[�g
		if(def_l >= 115){
			ecrobot_sound_tone(277, 100, 50);
			changeSection(&buf_x, &buf_y, &buf_l, &buf_th);
			crt_sect = SND_CORNER;
			trgt_speed = trgt_speed +10;
		}
		trgt_R = 0.0;
		break;
	case (SND_CORNER):		//���X�g���[�g�I�_�����J�[�u
		if(def_x <= -95 && def_y <= -5 && def_l >= 245 && def_th >= 240){
			ecrobot_sound_tone(293, 100, 50);
			changeSection(&buf_x, &buf_y, &buf_l, &buf_th);
			crt_sect = SND_STRAIGHT;
			trgt_speed = trgt_speed;
		}
		trgt_R = 56.59;
		break;
	case (SND_STRAIGHT):	//���J�[�u�I�_�����X�g���[�g
		if(def_x >= 40 && def_y <= -15 && def_l >= 40){
			ecrobot_sound_tone(311, 100, 50);
			changeSection(&buf_x, &buf_y, &buf_l, &buf_th);
			crt_sect = TRD_CORNER;
			trgt_speed = trgt_speed;
		}
		trgt_R = 0.0;
		break;
	case (TRD_CORNER):		//���X�g���[�g�I�_����O�J�[�u
		if(def_x <= -80 && def_y <= -80 && def_l >= 235 && def_th <= -210){
			ecrobot_sound_tone(329, 100, 50);
			changeSection(&buf_x, &buf_y, &buf_l, &buf_th);
			crt_sect = TRD_STRAIGHT;
			trgt_speed = trgt_speed;
		}
		trgt_R = -64.02;
		break;
	case (TRD_STRAIGHT):	//��O�J�[�u�I�_����O�X�g���[�g
		if(def_x <= -50 && def_y >= 105 && def_l >= 115){
			ecrobot_sound_tone(349, 100, 50);
			changeSection(&buf_x, &buf_y, &buf_l, &buf_th);
			crt_sect = FIN_APPROACH;
			trgt_speed = trgt_speed;
		}
		trgt_R = 0.0;
		break;
	case (FIN_APPROACH):	//��O�X�g���[�g�I�_���}�[�J�[
		if(1){
			ecrobot_sound_tone(369, 100, 50);
			trgt_speed = trgt_speed;
		}
		trgt_R = 51.80;
		break;
	case (STEPS):			//�K�i
		if(1){
			ecrobot_sound_tone(369, 100, 50);
		}
		trgt_R = 51.80;
		break;
	case (TURN90D):
		if(1){
			ecrobot_sound_tone(369, 100, 50);
		}
		trgt_R = 51.80;
		break;
	default:
		trgt_speed = 0;
		trgt_R = 0.0;
		break;
	}
}

void changeSection(float *buf_x, float *buf_y, float *buf_l, float *buf_th){
	*buf_x =  getXCoo();	*buf_y =  getYCoo();
	*buf_l = getDistance();	*buf_th = getTheta( );
}

/* Marker detector */
unsigned char MKTrigger(){
	//* MKflg*
	// 0: �}�[�J�[�ł͂Ȃ�
	// 1: �}�[�J�[�ł���
	// 2: �}�[�J�[��������Ȃ�
	unsigned char MKflg = 0;
	static signed char LV_record[10] = {0, 0, 0, 0, 0, 0, 0, 0, 0, 0};
	static unsigned char i = 0;

	LV_record[i] = LVTrigger();

	//* flgs *
	// p_flg: ���̐M�����ϑ����ꂽ
	// m_flg: ���̐M�����ϑ����ꂽ
	// P_flg: ���̐M�����Q��ȏ�A���Ŋϑ����ꂽ
	// M_flg: ���̐M�����Q��ȏ�A���Ŋϑ����ꂽ
	unsigned char p_flg = 0, M_flg = 0, m_flg = 0, P_flg = 0;
	unsigned char j, k;
	for( j=0; j<10; j++){
		if(j+i < 10){
			k = 0;
		}
		else{
			k = i;
		}

		switch(LV_record[i+j-k]){
		case 1:
			if(p_flg == 1 && M_flg == 1){
				MKflg = 1;
			}
			else if(p_flg ==1 && M_flg == 0){
				MKflg = 2;	P_flg = 1;
			}
			p_flg = 1;	m_flg = 0;
			break;
		case -1:
			if(m_flg == 1 && P_flg == 1){
				MKflg = 1;
			}
			else if(m_flg == 1 && P_flg == 0){
				MKflg = 2;	M_flg = 1;
			}
			p_flg = 0;	m_flg = 1;
			break;
		default:
			m_flg = p_flg = 0;
			break;
		}
		if(MKflg == 1){
			break;
		}
	}

	if(i++ >= 10){
		i = 0;
	}
	return MKflg;
}

/* ���Z���T�l�̋}���ȏ㏸���~�����m������ */
signed char LVTrigger(){
	signed int LV_def = ecrobot_get_light_sensor(NXT_PORT_S3) - LV_buf;
	LV_buf = ecrobot_get_light_sensor(NXT_PORT_S3);
	if(LV_def >= 5){
		return 1;
	}
	else if(LV_def <= -5){
		return -1;
	}
	else {
		return 0;
	}
}


 float getTargetR(){

	 return trgt_R;
 }

S8 getTargSpeed(){
	 return trgt_speed;
 }

void bufClear(){
	  buf_x = 0.0;
	  buf_y = 0.0; 
	  buf_l = 0.0; 
	  buf_th = 0.0;
  }
/*
 * runner.c
 * �x�[�V�b�N�R�[�X���s�v���O����
 */


#include "runnerForTestRun.h"
#include "mymath.h"
#include "SelfLocation.h"
#include "Calibration.h"
#include "MainRunningInCourse.h"


void cngSection(IN_SECTION *crt_sect, float *buf_l, int tone); 



//�ȗ����aPID����֐�
int RA_curvatureCtrl_PID(float target) {
	static float bf_dev = 0.0;

	float dev = R - target;
	float i_dev = i_dev + (dev * 0.004);
	float d_dev = (dev - bf_dev) / 0.004;
	bf_dev = dev;

	S8 turn = RKp * dev + RKi * i_dev + RKd * d_dev;
	//S8 turn = 0.6 * dev;
	if (-100 > turn) {
		turn = -100;
	}
	else if (100 < turn) {
		turn = 100;
	}

	return turn;
}

//�ԑ̕���PID����֐�
int RA_directionCtrl_PID(float target) {
	static float bf_dev = 0.0;
	
	float dev = getTheta( ) - target;
	//float i_dev = i_dev + (dev * 0.0005);
	float d_dev = (dev - bf_dev) / 0.004;
	bf_dev = dev;

	//S8 turn = Kp * dev + Ki * i_dev + Kd * d_dev;
	S8 turn = 1.2 * dev + 0.5 * d_dev;
	if (-100 > turn) {
		turn = -100;
	}
	else if (100 < turn) {
		turn = 100;
	}
	//�ڕW�p�x��-1�x���w�肳�ꂽ��ԑ̊p�x����͖����ɂ���
	if(target ==-1){
		turn = 0;
	}
	return turn;
}


/*test code for �ȗ��P�x�n�C�u���b�hPID����  
//���s��Ԑݒ�֐�
void RN_setting(){
	static unsigned char buf = 0;
	if (ecrobot_get_touch_sensor(NXT_PORT_S4)){
		buf = 1;
	}
	if (!ecrobot_get_touch_sensor(NXT_PORT_S4) && buf){
		runner_mode = RN_MODE_INIT;
		setting_mode = RN_SETTINGMODE_START;
	}

	switch (setting_mode){
	//�L�����u���[�V�������
	case (RN_SETTINGMODE_START):
		RN_calibrate();
		LV_buf = ecrobot_get_light_sensor(NXT_PORT_S3);
		break;
		
	
	//�ʏ푖�s��� �L�����u���[�V�������I����RN_RUN��ԂɑJ�ځ@wait_count �̏����l�̓[���ő��肾���Ă���J�E���g��������B
	case (RN_RUN):
		RA_linetrace_PID_balanceoff(SPEED_COUNT);

		cmd_turn += RA_curvatureCtrl_PID(trgt_R);
		wait_count++;
		break;
	
	// �ȗ�PID
	case (RN_CVRUN):
		cmd_forward = SPEED_COUNT;
		cmd_turn = RA_curvatureCtrl_PID(trgt_R);
		wait_count++;
		break;
		
	case (RN_LOT90):
		cmd_turn = 0;
		cmd_turn = RA_directionCtrl_PID(90);
		wait_count++;
		break;

	default:
		break;
	}
}
*/




/*



/* �C���R�[�X���s��Ԍ��o */
void setSection_in(){
	static int wait_count = 0;
	wait_count++;	
	float def_x  =  getXCoo() - buf_x;
	float def_y  =  getYCoo() - buf_y;
	float def_l  = getDistance() - buf_l;
	float def_th = getTheta( ) - buf_th;

	switch(crt_sect){
	case (START):			//�X�^�[�g���⓹
		if(getInitGyroOffset()-20 /*- 30*/ > (U32)ecrobot_get_gyro_sensor(NXT_PORT_S1) && wait_count > 500){
			ecrobot_sound_tone(220, 100, 50);
			theta=0; //�p�x�␳
			x_r=0;
			y_r=185;
			changeSection(&buf_x, &buf_y, &buf_l, &buf_th);
			crt_sect = UP_SLOPE;
			trgt_speed = trgt_speed;
			trgt_theta = 0;
			
		}
		trgt_R = 0.0;
		break;

		case (UP_SLOPE):		//�⓹�n�_��37cm//���_
			if(def_l >= 25){
		//if(def_l >= 57/*65*/ && getInitGyroOffset() - 20 > (U32)ecrobot_get_gyro_sensor(NXT_PORT_S1)){

			ecrobot_sound_tone(233, 100, 50);
			changeSection(&buf_x, &buf_y, &buf_l, &buf_th);
			crt_sect = SLOPE_SLOW_DOWN;
			trgt_speed = trgt_speed -30;
			theta=0; //�p�x�␳
		}
		trgt_R = 0.0;
		trgt_theta = 0;
		
		break;

		case (SLOPE_SLOW_DOWN):

		if(/*def_l >= 5765*/ /*&&*/ getInitGyroOffset() - 7 > (U32)ecrobot_get_gyro_sensor(NXT_PORT_S1)){
		
			//if(def_l >= 30){
			

			ecrobot_sound_tone(833, 100, 100);
			changeSection(&buf_x, &buf_y, &buf_l, &buf_th);
			crt_sect = DOWN_SLOPE;
			GRAY_VALUE -=20;
			theta=0; //�p�x�␳
		}
		trgt_R = 0.0;
		
		break;

	
	
		break;
	case (DOWN_SLOPE):	
		//���_���⓹�I�_,�⓹�㕽�n�X�g���[�g
		//if(getDistance() >= 390){
		if(getInitGyroOffset() - 10 >(U32)ecrobot_get_gyro_sensor(NXT_PORT_S1)){
			//ecrobot_sound_tone(246, 100, 50);
			changeSection(&buf_x, &buf_y, &buf_l, &buf_th);
			crt_sect = MINI_STRARIGHT;
			trgt_speed = trgt_speed + 30;	
			GRAY_VALUE +=20;
			theta=0; //�p�x�␳
		}
		trgt_R = 0.0;
		trgt_theta = 0;
		TailAngleChange(ANGLEOFSLOPEDOWN);
		break;
	case (MINI_STRARIGHT):
			if(def_l >= 25/*80*/){
			ecrobot_sound_tone(246, 100, 50);
			changeSection(&buf_x, &buf_y, &buf_l, &buf_th);
			crt_sect = FST_CORNER;
			theta=0; //�p�x�␳
			}

		break;
	case (FST_CORNER):		//�⓹�I�_�����J�[�u
		
		if(/*def_x >= 74 && def_y >= 70 &&*/ def_l >= 110 && def_th >= 90){
			ecrobot_sound_tone(261, 100, 100);
			changeSection(&buf_x, &buf_y, &buf_l, &buf_th);
			crt_sect = FST_STRAIGHT;
			trgt_speed = trgt_speed +10;
			theta=90; //�p�x�␳
		}
		trgt_theta = -1;
		trgt_R = 67.59;
		TailAngleChange(ANGLEOFDOWN);
		ecrobot_sound_tone(246, 100, 50);
		break;

	case (FST_STRAIGHT):	//���J�[�u�I�_�����X�g���[�g
		if(def_l >= 115){
			ecrobot_sound_tone(277, 100, 100);
			changeSection(&buf_x, &buf_y, &buf_l, &buf_th);
			crt_sect = SND_CORNER;
			trgt_speed = trgt_speed -10;
		}
		trgt_R = 0.0;
		break;
	case (SND_CORNER):		//���X�g���[�g�I�_�����J�[�u
		if(def_th >= 240){
		ecrobot_sound_tone(293, 100, 100);
			//if(def_x <= -95 && def_y <= -5 && def_l >= 245 && def_th >= 240){
			ecrobot_sound_tone(293, 100, 100);
			changeSection(&buf_x, &buf_y, &buf_l, &buf_th);
			crt_sect = SND_STRAIGHT;
			trgt_speed = trgt_speed +10;
		}
		trgt_R = 56.59;
		trgt_theta = -1;
		break;
	case (SND_STRAIGHT):	//���J�[�u�I�_�����X�g���[�g
		
		if(/*def_x >= 40 && def_y <= -15 &&*/ def_l >= 40){
			ecrobot_sound_tone(311, 100, 100);
			changeSection(&buf_x, &buf_y, &buf_l, &buf_th);
			crt_sect = TRD_CORNER;
			trgt_speed = trgt_speed -10;
		}
		trgt_R = 0.0;
		trgt_theta = -1;/*-45;*/ //���m�ɑ���K�v�L��
		break;
	case (TRD_CORNER):		//���X�g���[�g�I�_����O�J�[�u
		if(def_l >= 235 && def_th <= -210){
		
		//if(def_x <= -80 && def_y <= -80 && def_l >= 235 && def_th <= -210){
			ecrobot_sound_tone(329, 100, 100);
			changeSection(&buf_x, &buf_y, &buf_l, &buf_th);
			crt_sect = TRD_STRAIGHT;
			trgt_speed = trgt_speed +10;
		}
		trgt_R = -64.02;
		trgt_theta = -1;
		break;
	case (TRD_STRAIGHT):	//��O�J�[�u�I�_����O�X�g���[�g
		if( def_l >= 95){
		
		//if(def_x <= -50 && def_y >= 105 && def_l >= 115){
			ecrobot_sound_tone(349, 100, 100);
			changeSection(&buf_x, &buf_y, &buf_l, &buf_th);
			crt_sect = FIN_APPROACH;
			trgt_speed = trgt_speed -10;
		}
		trgt_R = 0.0;
		trgt_theta = -1; //���m�ɑ����ē��͂���
		break;
	case (FIN_APPROACH):	//��O�X�g���[�g�I�_���}�[�J�[
			if(def_l >=80){
	
	//	if(def_l >= 80){
				cngSection(&crt_sect, &buf_l, 220);
		ecrobot_sound_tone(369, 100, 100);
			trgt_speed = trgt_speed +0;
		GRAY_VALUE -=40;
		}
		trgt_speed = 30;
		trgt_R = 51.80;
		trgt_theta = -1;
		break;

//�ȉ��A�{�[�i�X�X�e�[�W
	case (B_FST_STRAIGHT):
		if(def_l >= 34){
		
		//if(def_l >= 64){
			cngSection(&crt_sect, &buf_l, 220);
			GRAY_VALUE +=40;
		ecrobot_sound_tone(869, 100, 50);
		}
		trgt_speed = 20;
		
		//Kp = 1.85;			//P����p
		//Ki = 1.6;			//I����p
		//Kd = 0.003;			//D����p
	//targCtrlMethod = CURV_PID;
		trgt_R = 0.0;
		trgt_theta = -1;

		break;
	case (B_FST_CORNER):
		if(def_l >= 70){
			targCtrlMethod = HYBRID;
			cngSection(&crt_sect, &buf_l, 233);
		}
		trgt_R = 30.0;
		break;
	case (B_SND_STRAIGHT):
		if(def_l >= 66){
			cngSection(&crt_sect, &buf_l, 246);
		}
		trgt_R = 0.0;
		break;
	case (B_SND_CORNER):
		if(def_l >= 44){
			cngSection(&crt_sect, &buf_l, 261);
		}
		trgt_R = -30.0;
		break;
	case (B_TRD_STRAIGHT):
		if(def_l >= 107){
			cngSection(&crt_sect, &buf_l, 277);
		}
		trgt_R = 0.0;
		break;
	case (B_TRD_CORNER):
		if(def_l >= 21){
			cngSection(&crt_sect, &buf_l, 293);
		}
		trgt_R = -30.0;
		break;
	case (LOOKUP):
		if(def_l >= 203){
			cngSection(&crt_sect, &buf_l, 311);
		}
		trgt_R = 0.0;
		break;

	default:
		trgt_speed = 0;
		trgt_R = 0.0;
		break;
	}
}



/* ______________�A�E�g�R�[�X���s��Ԍ��o_______________________ */
void setSection_out(){
	
/* �A�E�g�R�[�X���s��� */
typedef enum{
	START,
	UP_SLOPE,
	DOWN_SLOPE,
	FST_CORNER,
	FST_STRAIGHT,
	SND_CORNER,
	SND_STRAIGHT,
	TRD_CORNER,
	TRD_STRAIGHT,
	FIN_APPROACH,
	MARKER,
	STEPS,
	TURN90D
} SECT_OUT;

	static int wait_count = 0;
	wait_count++;
	static SECT_OUT crt_sect = START;
	
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
		}
		trgt_R = 0.0;
		break;
	case (UP_SLOPE):		//�⓹�n�_�����_
		if(def_l >= 30 && getInitGyroOffset() - 30 > (U32)ecrobot_get_gyro_sensor(NXT_PORT_S1)){
			ecrobot_sound_tone(233, 100, 50);
			changeSection(&buf_x, &buf_y, &buf_l, &buf_th);
			crt_sect = DOWN_SLOPE;
		}
		trgt_R = 0.0;
		break;
	case (DOWN_SLOPE):		//���_���⓹�I�_
		//if(getDistance() >= 390){
		if(def_l >= 90){
			ecrobot_sound_tone(246, 100, 50);
			changeSection(&buf_x, &buf_y, &buf_l, &buf_th);
			crt_sect = FST_CORNER;
		}
		trgt_R = 0.0;
		break;
	case (FST_CORNER):		//�⓹�I�_�����J�[�u
		if(def_th >= 80){
		
		//if(def_x >= 74 && def_y >= 70 && def_l >= 110 && def_th >= 90){
			ecrobot_sound_tone(261, 100, 50);
			changeSection(&buf_x, &buf_y, &buf_l, &buf_th);
			crt_sect = FST_STRAIGHT;
		}
		
		trgt_R = 67.59;
		break;
	case (FST_STRAIGHT):	//���J�[�u�I�_�����X�g���[�g
		if(def_l >= 115){
			ecrobot_sound_tone(277, 100, 50);
			changeSection(&buf_x, &buf_y, &buf_l, &buf_th);
			crt_sect = SND_CORNER;
		}
		trgt_R = 0.0;
		ecrobot_sound_tone(246, 100, 100);

		break;
	case (SND_CORNER):		//���X�g���[�g�I�_�����J�[�u
		if(def_x <= -95 && def_y <= -5 && def_l >= 245 && def_th >= 240){
			ecrobot_sound_tone(293, 100, 50);
			changeSection(&buf_x, &buf_y, &buf_l, &buf_th);
			crt_sect = SND_STRAIGHT;
		}
		trgt_R = 56.59;
		changeSection(&buf_x, &buf_y, &buf_l, &buf_th);
		break;
	case (SND_STRAIGHT):	//���J�[�u�I�_�����X�g���[�g
		if( def_l >= 40){
	
	//	if(def_x >= 40 && def_y <= -15 && def_l >= 40){
			ecrobot_sound_tone(311, 100, 50);
			changeSection(&buf_x, &buf_y, &buf_l, &buf_th);
			crt_sect = TRD_CORNER;
		}
		trgt_R = 0.0;
		break;
	case (TRD_CORNER):		//���X�g���[�g�I�_����O�J�[�u
		if(def_x <= -80 && def_y <= -80 && def_l >= 235 && def_th <= -210){
			ecrobot_sound_tone(329, 100, 50);
			changeSection(&buf_x, &buf_y, &buf_l, &buf_th);
			crt_sect = TRD_STRAIGHT;
		}
		trgt_R = -64.02;
		break;
	case (TRD_STRAIGHT):	//��O�J�[�u�I�_����O�X�g���[�g
		if(def_x <= -50 && def_y >= 105 && def_l >= 115){
			ecrobot_sound_tone(349, 100, 50);
			changeSection(&buf_x, &buf_y, &buf_l, &buf_th);
			crt_sect = FIN_APPROACH;
		}
		trgt_R = 0.0;
		break;
	case (FIN_APPROACH):	//��O�X�g���[�g�I�_���}�[�J�[
		if(1){
			ecrobot_sound_tone(369, 100, 50);
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

void cngSection(IN_SECTION *crt_sect, float *buf_l, int tone){
	ecrobot_sound_tone(tone, 100, 50);
	*crt_sect += 1;
	*buf_l = getDistance();	
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
	  trgt_speed = TARGET_SPEED;
  }

S8  getTargetTheta(){
	return trgt_theta;
 }
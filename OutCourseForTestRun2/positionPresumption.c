#include "positionPresumption.h"

/*
 * markerdetection.c
 * �}�[�J�[���o�v���O����
 */


#include "positionPresumption.h"
#include "math.h"


/*
 *	�e���`
*/

#define PI 3.141592

//�K���ݒ�p�x
#define ANGLEOFDOWN 112				//�~���ڕW�p�x
#define ANGLEOFUP 6					//�㏸�ڕW�p�x

//���x�J�E���^�̏���l
#define SPEED_COUNT 20

//�W���C���U���l
#define PM_GYRO 65

/* �ԗ֔��a�A���s�̕�*/
#define W_RADIUS	4.1	//[mm]
#define W_DIST		16.2//[mm]

//�����[�g�X�^�[�g�R�}���h(�ύX�֎~)
#define CMD_START '1'

/*
 *	�O���[�o���ϐ�
 */

//���C���g���[�X�p�ڕW�l
static unsigned int BLACK_VALUE;	//���l
static unsigned int WHITE_VALUE;	//���l
static unsigned int GRAY_VALUE;		//�D�F�l�i���݂͍��Ɣ��̕��ϒl�j
//static unsigned int LOOKUP_BLACK_VALUE;		//�p�x��ANGLEOFLOOKUP���̍��l
//static unsigned int LOOKUP_WHITE_VALUE;		//�p�x��ANGLEOFLOOKUP���̔��l
static unsigned int LOOKUP_GRAY_VALUE;		//�p�x��ANGLEOFLOOKUP�n�̊D�F�l�i���݂͍��Ɣ��̕��ϒl�j

//static unsigned int LOOKUP_SONAR_VALUE;

//���x���߃J�E���^�i�J�E���^������ɒB���閈�ɑ��x��1�㏸
static int speed_counter = 0;


//PID����p�΍��l
static float hensa;					//P����p
static float i_hensa = 0;			//I����p
static float d_hensa = 0;			//D����p
static float bf_hensa = 0;


//���C���g���[�X��PID����p�W��
static float Kp = 1.85;				//P����p
static float Ki = 1.6;				//I����p
static float Kd = 0.003;			//D����p

//�K��PI����p�W��
static float t_Kp = 1.85;			//P����p
static float t_Ki = 0;				//I����p

//�K������p�ϐ�
static int t_angle = 0;				//���݂̊p�x
static int t_count = 0;				//���䑬�x���ߗp�J�E���^
static int t_value = 0;				//�p�x�ڕW�l
static int t_count_limit = 0;		//�J�E���^�ő�l
static int t_up = 0;				//�����l

//�S�̗p�J�E���^�i���ԉ҂��p�j
static int wait_count = 0;

//�W���C���Z���T�I�t�Z�b�g�v�Z�p�ϐ�
//static U32	gyro_offset = 0;    /* gyro sensor offset value */

/* ���Ȉʒu����p�ϐ� */
static float x_r = 0;		//�ԑ̂�X���W
static float y_r = 0;		//�ԑ̂�Y���W
static float dist = 0;		//�ړ�����
static float theta = 0;		//�ԑ̂̊p�x
static float R = 0;			//�ȗ����a

char rx_buf[BT_MAX_RX_BUF_SIZE];

/* �o�����X�R���g���[���֓n���R�}���h�p�ϐ� */
S8  cmd_forward, cmd_turn;
/* �o�����X�R���g���[������Ԃ���郂�[�^����p�ϐ� */
S8	pwm_l, pwm_r;

//�ڕW���x
static S8 trgt_speed = 100;


//�ڕW�ȗ����a
static float trgt_R = 0;

//�}�[�J�[�t���O�@0: OFF, 1: ON
unsigned char m_flg = 0;
static unsigned int LV_buf = 0;		/* Light Value buffer */

/*
 *	�e���Ԓ�`
 */

//�V�X�e���S�̂̏��
typedef enum{
	RN_MODE_INIT, 					//�������
	RN_MODE_BALANCE,				//�|������ON
	RN_MODE_BALANCEOFF				//�|������OF
} RN_MODE;


//�L�����u���[�V�����̏��
typedef enum{
	RN_SETTINGMODE_START,		//�������
	RN_SPEEDZERO,				//���x�[���L�[�v
	RN_RUN,						//��{���s�i���C���g���[�X�j
	RN_LOOKUP,					//���b�N�A�b�v�Q�[�g����
	RN_LOOKUPDOWN,				//���s�̍~��
	RN_LOOKUPMOVE,				//���s�̑O�i
	RN_LOOKUPUP,				//���s�̕��A
	RN_CVRUN,					//�ȗ�PID
	RN_LOT90					//90�x����i�K�i�j
} RN_SETTINGMODE;

//�K���̏��
typedef enum{
	RN_TAILDOWN,				//�K���~��
	RN_TAILUP,					//�K���㏸
} RN_TAILMODE;

/* ���s��� */
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
	B_FST_STRAIGHT,
	B_FST_CORNER,
	B_SND_STRAIGHT,
	B_SND_CORNER,
	B_TRD_STRAIGHT,
	B_TRD_CORNER,
	LOOKUP
} IN_SECTION;

//�������
RN_MODE runner_mode = RN_MODE_INIT;
RN_SETTINGMODE setting_mode = RN_SETTINGMODE_START;
RN_TAILMODE tail_mode = RN_TAILUP;

/*
 *	�e��v���C�x�[�g�֐���`
 */


//�e��v���C�x�[�g�֐�
void RN_setting();

void RN_modesetting();


int RA_directionCtrl_PID(float target);
int RA_curvatureCtrl_PID(float target);

//void self_location(void);
//void setSection();void changeSection(float *buf_x, float *buf_y, float *buf_l, float *buf_th);
//unsigned char MKTrigger(void);
//signed char LVTrigger(void);
int abs(int n);
float deg2rad(float degree);
float rad2deg(float radian);

//�ȗ����aPID����֐�
int RA_curvatureCtrl_PID(float target) {
	static float bf_dev = 0.0;

	float dev = R - target;
	//float i_dev = i_dev + (dev * 0.0005);
	//float d_dev = (dev - bf_dev) / 0.0005;
	bf_dev = dev;

	//S8 turn = Kp * dev + Ki * i_dev + Kd * d_dev;
	int turn = 0.6 * dev;
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
	
	float dev = theta - target;
	//float i_dev = i_dev + (dev * 0.0005);
	float d_dev = (dev - bf_dev) / 0.0005;
	bf_dev = dev;
	
	//S8 turn = Kp * dev + Ki * i_dev + Kd * d_dev;
	int turn = 1.0 * dev + 0.5 * d_dev;
	if (-100 > turn) {
		turn = -100;
	}
	else if (100 < turn) {
		turn = 100;
	}

	return turn;
}


//���s��Ԑݒ�֐�
void RN_setting(){

	switch (setting_mode){
	//�L�����u���[�V�������
	case (RN_SETTINGMODE_START):
		RN_calibrate();
		LV_buf = ecrobot_get_light_sensor(NXT_PORT_S3);
		break;
		
	//�ʏ푖�s���
	case (RN_RUN):
		RA_linetrace_PID(SPEED_COUNT);
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

/*
//���Ȉʒu����֐�
void self_location(){
	static float dist_t = 0.0;
	static float theta_t = 0.0;

	float fi_l = (float)nxt_motor_get_count(NXT_PORT_C);
	float fi_r = (float)nxt_motor_get_count(NXT_PORT_B);
	float dist_l = deg2rad(fi_l * W_RADIUS);
	float dist_r = deg2rad(fi_r * W_RADIUS);

	dist = (dist_l + dist_r) / 2.0;
	theta = W_RADIUS / W_DIST * (fi_r - fi_l);
	x_r += (dist - dist_t) * sin(deg2rad(theta));
	y_r += (dist - dist_t) * cos(deg2rad(theta));
	if(!(theta == theta_t)){
		R = rad2deg((dist - dist_t) / (theta - theta_t));
	}
	else{
		R = 0.0;
	}
		
	dist_t = dist;
	theta_t = theta;
}
*/
float deg2rad(float degree){
	float radian = PI / 180.0 * degree;
	return radian;
}

float rad2deg(float radian){
	float degree = 180.0 / PI * radian;
	return degree;
}

/* ���s��� */
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
	B_FST_STRAIGHT,
	B_FST_CORNER,
	B_SND_STRAIGHT,
	B_SND_CORNER,
	B_TRD_STRAIGHT,
	B_TRD_CORNER,
	LOOKUP
} IN_SECTION;

/* ���s��Ԍ��o */
/*
void setSection(){
	static IN_SECTION crt_sect = START;
	
	static float buf_l = 0.0;
	float def_l  = dist - buf_l;

	switch(crt_sect){
	case (START):			//�X�^�[�g���⓹
		if(GYRO_OFFSET - 30 > (U32)ecrobot_get_gyro_sensor(NXT_PORT_S1) && wait_count > 500){
			cngSection(&crt_sect, &buf_l, 220);
		}
		trgt_R = 0.0;
		break;
	case (UP_SLOPE):		//�⓹�n�_�����_
		if(def_l >= 30){
			cngSection(&crt_sect, &buf_l, 233);
		}
		trgt_R = 0.0;
		break;
	case (DOWN_SLOPE):		//���_���⓹�I�_
		//if(dist >= 390){
		if(def_l >= 90){
			cngSection(&crt_sect, &buf_l, 246);
		}
		trgt_speed = 60;
		Kp = 1.85;			//P����p
		Ki = 1.6;			//I����p
		Kd = 0.003;			//D����p

		trgt_R = 0.0;
		break;
	case (FST_CORNER):		//�⓹�I�_�����J�[�u
		if(def_l >= 110){
			cngSection(&crt_sect, &buf_l, 261);
		}
		trgt_speed = 100;
		Kp = 0.456;			//P����p
		Ki = 1.61;			//I����p
		Kd = 0.017;			//D����p
		trgt_R = 67.59;
		break;
	case (FST_STRAIGHT):	//���J�[�u�I�_�����X�g���[�g
		if(def_l >= 115){
			cngSection(&crt_sect, &buf_l, 277);
		}
		trgt_R = 0.0;
		break;
	case (SND_CORNER):		//���X�g���[�g�I�_�����J�[�u
		if(def_l >= 245){
			cngSection(&crt_sect, &buf_l, 293);
		}
		trgt_R = 56.59;
		break;
	case (SND_STRAIGHT):	//���J�[�u�I�_�����X�g���[�g
		if(def_l >= 40){
			cngSection(&crt_sect, &buf_l, 311);
		}
		trgt_R = 0.0;
		break;
	case (TRD_CORNER):		//���X�g���[�g�I�_����O�J�[�u
		if(def_l >= 235){
			cngSection(&crt_sect, &buf_l, 329);
		}
		trgt_R = -64.02;
		break;
	case (TRD_STRAIGHT):	//��O�J�[�u�I�_����O�X�g���[�g
		if(def_l >= 115){
			cngSection(&crt_sect, &buf_l, 349);
		}
		trgt_R = 0.0;
		break;
	case (FIN_APPROACH):	//��O�X�g���[�g�I�_���}�[�J�[
		if(def_l >= 66){
			cngSection(&crt_sect, &buf_l, 369);
		}
		trgt_R = 51.80;
		break;
	//�ȉ��A�{�[�i�X�X�e�[�W
	case (B_FST_STRAIGHT):
		if(def_l >= 64){
			cngSection(&crt_sect, &buf_l, 220);
		}
		trgt_speed = 60;
		Kp = 1.85;			//P����p
		Ki = 1.6;			//I����p
		Kd = 0.003;			//D����p

		trgt_R = 0.0;
		break;
	case (B_FST_CORNER):
		if(def_l >= 70){
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
*/
/*
void cngSection(IN_SECTION *crt_sect, float *buf_l, int tone){
	ecrobot_sound_tone(tone, 100, 50);
	*crt_sect += 1;
	*buf_l = dist;
}
*/
/*
void changeSection(float *buf_x, float *buf_y, float *buf_l, float *buf_th){
	*buf_x = x_r;	*buf_y = y_r;
	*buf_l = dist;	*buf_th = theta;
}
*/
/******************************** END OF FILE ********************************/

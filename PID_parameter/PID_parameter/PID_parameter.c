/*
 *	PID_parameter.c
 *	PID�p�����[�^���ߗp�v���O����
 */

#include "PID_parameter.h"
#include "logSend.h"
#include "math.h"
#include "tyreal_light_ver.h"


/*
 *	�e��ϐ���`
 */

//���C���g���[�X�p�ڕW�l
static unsigned int BLACK_VALUE;	//���l
static unsigned int WHITE_VALUE;	//���l
static unsigned int GRAY_VALUE;		//�D�F�l�i���݂͍��Ɣ��̕��ϒl�j

static int counter = 0;

//�K���ݒ�p�x
#define ANGLEOFDOWN 95			//�~���ڕW�p�x
#define ANGLEOFUP 0					//�㏸�ڕW�p�x

#define CMD_STOP '3'

#define ANGLE_OF_AIM 180  //�E�������p�x

//���x���ߌW��
#define SPEED_COUNT 10

//���C���g���[�X��PID����p�W��

static float Kp = 0.436;//0.436;				//P����p
static float Ki = 0;//1.31;					//I����p
static float Kd = 0.074;//0.011;				//D����p

//�W���C���Z���T�I�t�Z�b�g�v�Z�p�ϐ�
static U32	gyro_offset = 0;    /* gyro sensor offset value */ 

char rx_buf[BT_MAX_RX_BUF_SIZE];

/* �o�����X�R���g���[���֓n���R�}���h�p�ϐ� */
S8  cmd_forward, cmd_turn;
/* �o�����X�R���g���[������Ԃ���郂�[�^����p�ϐ� */
S8	pwm_l, pwm_r;

/*
 *	��Ԓ�`
 */

//�V�X�e���S�̂̏��
typedef enum{
	RN_MODE_INIT, 					//�������
	RN_MODE_BALANCE,				//�|������ON
	RN_MODE_BALANCEOFF,				//�|������OFF
} RN_MODE;


typedef enum{
	RN_TYREAL,
	RN_SETTINGMODE_START,		//�������
	RN_RUN,						//��{���s�i���C���g���[�X�j
	RN_RIGHT,
} RN_SETTINGMODE;


//�K���̏��
typedef enum{
	RN_TAILDOWN,				//�K���~��
	RN_TAILUP,					//�K���㏸
} RN_TAILMODE;


//�������
RN_MODE runner_mode = RN_MODE_INIT;
RN_SETTINGMODE setting_mode = RN_SETTINGMODE_START;
//RN_SETTINGMODE setting_mode = RN_TYREAL;

RN_TAILMODE tail_mode = RN_TAILDOWN;

/*	
 *	�e��֐���`
 */

//�e��v���C�x�[�g�֐�
void RN_calibrate();
void RN_setting();
int online();
void RA_linetrace(int forward_speed, int turn_speed);
void RA_linetrace_PID(int forward_speed);
void tailcontrol();
int RA_speed(int forward_speed);
void RN_modesetting();
static int remote_start(void);
static int remote_stop(void);
static int right(void);

//�J�E���^�̐錾
DeclareCounter(SysTimerCnt);

//�t���f�B�X�v���C�ɕ\������V�X�e�����ݒ�
const char target_subsystem_name[] = "PID";


/*
 *	�e��֐�
 */

//��������
void ecrobot_device_initialize(void)
{
	ecrobot_set_light_sensor_active(NXT_PORT_S3);
	ecrobot_init_bt_slave("LEJOS-OSEK");

	ecrobot_set_motor_rev(NXT_PORT_A,0);
	ecrobot_set_motor_rev(NXT_PORT_B,0);
	ecrobot_set_motor_rev(NXT_PORT_C,0);
}


//��n������
void ecrobot_device_terminate(void)
{
	tail_mode = RN_TAILUP;
	ecrobot_set_motor_speed(NXT_PORT_A, 0);
	ecrobot_set_motor_speed(NXT_PORT_B, 0);
	ecrobot_set_motor_speed(NXT_PORT_C, 0);

	ecrobot_set_light_sensor_inactive(NXT_PORT_S3);
	ecrobot_term_bt_connection();
}


//OSEK�t�b�N�֐�
void StartupHook(void){}
void ShutdownHook(StatusType ercd){}
void PreTaskHook(void){}
void PostTaskHook(void){}
void ErrorHook(StatusType ercd){}


//�t�b�N�֐�
void user_1ms_isr_type2(void){
	StatusType ercd;
	/*
	 *  Increment OSEK Alarm System Timer Count
     */
	ercd = SignalCounter( SysTimerCnt );
	if( ercd != E_OK ){
		ShutdownOS( ercd );
	}
}


//PID���䃉�C���g���[�X�֐�
void RA_linetrace_PID(int forward_speed) {

	//PID����p�΍��l
	static float hensa;					//P����p
	static float i_hensa = 0;			//I����p
	static float d_hensa = 0;			//D����p
	static float bf_hensa = 0;
	float right_motor_turn=0,left_motor_turn;
	
	forward_speed = RA_speed(forward_speed);	//���x��i�K�I�ɕω�

	if(forward_speed > 0)
		hensa = (float)GRAY_VALUE - (float)ecrobot_get_light_sensor(NXT_PORT_S3);
	else
		hensa = (float)ecrobot_get_light_sensor(NXT_PORT_S3) - (float)GRAY_VALUE;

	i_hensa = i_hensa+(hensa*0.0002);
	d_hensa = (hensa - bf_hensa)/0.0002;
	bf_hensa = hensa;

	cmd_turn = -(Kp * hensa + Ki * i_hensa + Kd * d_hensa);
	
//	cmd_turn = -(Kp * hensa);

	right_motor_turn = forward_speed - cmd_turn/2;
	left_motor_turn = forward_speed  + cmd_turn/2;

	if (-128 > right_motor_turn) {
		right_motor_turn = -128;
	} else if (127 < right_motor_turn) {
		right_motor_turn = 127;
	}

	if (-128 > left_motor_turn) {
		left_motor_turn = -128;
	} else if (127 < left_motor_turn) {
		left_motor_turn = 127;
	}

	pwm_l = left_motor_turn;
	pwm_r = right_motor_turn;

}


//�i�K�I�����p�֐��i�w��ʂ������x�����X�ɏ㏸�j
int RA_speed(int forward_speed){

	static int result_speed = 0;
	counter++;

	if(counter >= SPEED_COUNT)
	{
		if(forward_speed - result_speed >= 0){
			result_speed++;

			if(result_speed > forward_speed)
				result_speed = forward_speed;
		}
		else{
			result_speed--;

			if(result_speed < forward_speed)
				result_speed = forward_speed;
		}
		counter = 0;
	}

	return result_speed;
}

//�K���p�x�R���g���[���֐�
void tailcontrol(){

	static const float t_Kp = 1.85;

	static float t_hensa = 0;
	static float t_speed = 0;

	switch(tail_mode){
		case(RN_TAILDOWN):
			t_hensa = ANGLEOFDOWN - ecrobot_get_motor_rev(NXT_PORT_A);		//�K����������
			break;

		case(RN_TAILUP):
			t_hensa = ANGLEOFUP - ecrobot_get_motor_rev(NXT_PORT_A);		//�K�����グ��
			break;

		default:
			break;
	}

	t_speed = t_Kp*t_hensa;

	if (t_speed < -100)
		t_speed = -100;
	else if (t_speed > 100)
		t_speed = 100;

	ecrobot_set_motor_speed(NXT_PORT_A, t_speed);

}


//���s�ݒ�֐�
void RN_setting()
{
	int speed = 90;

	switch (setting_mode){

		case (RN_TYREAL):
			do_tyreal(&Kp,&Ki,&Kd);

			if(ecrobot_get_touch_sensor(NXT_PORT_S4) == TRUE)
			{
							ecrobot_set_motor_speed(NXT_PORT_A,0);
			ecrobot_set_motor_speed(NXT_PORT_B,0);
			ecrobot_set_motor_speed(NXT_PORT_C,0);
				ecrobot_sound_tone(932, 512, 20);
				systick_wait_ms(100);
				ecrobot_sound_tone(466, 256, 20);
				systick_wait_ms(10);
				systick_wait_ms(500);
				setting_mode = RN_SETTINGMODE_START;
			}
			break;
		
			//���s�J�n�O
		case (RN_SETTINGMODE_START):
			RN_calibrate();								//�L�����u���[�V����
			ecrobot_set_motor_speed(NXT_PORT_A,0);
			ecrobot_set_motor_speed(NXT_PORT_B,0);
			ecrobot_set_motor_speed(NXT_PORT_C,0);
			break;

			//�ʏ푖�s
		case (RN_RUN):
			RA_linetrace_PID(speed);
			if (remote_stop()==1)
			{	
				ecrobot_sound_tone(932, 512, 20);
				systick_wait_ms(100);
				ecrobot_sound_tone(466, 256, 20);
				systick_wait_ms(500);
 		 		//speed = 0;
				runner_mode = RN_MODE_BALANCEOFF;
				setting_mode = RN_TYREAL;
				
			}
			break;


			//�E������
		case(RN_RIGHT):
			right();
			break;

		default:
			break;
	}
}

//�E�������֐�
static int right()
{
	ecrobot_set_motor_rev(NXT_PORT_C, 0);
	ecrobot_set_motor_speed(NXT_PORT_C, 50);
	
	while(ecrobot_get_motor_rev(NXT_PORT_C) <= ANGLE_OF_AIM)
	{
	}
	
	ecrobot_set_motor_speed(NXT_PORT_C, 0);
}



//�L�����u���[�V�����֐�
void RN_calibrate()
{

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

	//���s�J�n���}
	while(1){

		//�^�b�`�Z���T�X�^�[�g
		if (ecrobot_get_touch_sensor(NXT_PORT_S4) == TRUE)
		{
			ecrobot_sound_tone(982,512,10);

			while(1){
					if (ecrobot_get_touch_sensor(NXT_PORT_S4) != TRUE)
					{
						setting_mode = RN_RUN;
						runner_mode = RN_MODE_BALANCEOFF;
						break;
					}
				}
			break;
		}
	}

}
//�����[�g�X�g�b�v�֐�
static int remote_stop(void)
{
	int i;
	unsigned int rx_len;
	unsigned char start = 0;

	for (i=0; i<BT_MAX_RX_BUF_SIZE; i++)
	{
		rx_buf[i] = 0; /* ��M�o�b�t�@���N���A */
	}

	rx_len = ecrobot_read_bt(rx_buf, 0, BT_MAX_RX_BUF_SIZE);
	if (rx_len > 0)
	{
		/* ��M�f�[�^���� */
		if (rx_buf[0] == CMD_STOP)
		{
			start = 1; /* ���s��~ */
		}
		ecrobot_send_bt(rx_buf, 0, rx_len); /* ��M�f�[�^���G�R�[�o�b�N */
	}

	return start;
}

//���s�̏�Ԑݒ�֐�
void RN_modesetting()
{
	switch (runner_mode){

			//�������
		case (RN_MODE_INIT):
			cmd_forward = 0;
			cmd_turn = 0;
			break;

			//�o�����T�[ON
		case (RN_MODE_BALANCE):
			balance_control(
				(F32)cmd_forward,
				(F32)cmd_turn,
				(F32)ecrobot_get_gyro_sensor(NXT_PORT_S1),
		 		(F32)gyro_offset,
				(F32)nxt_motor_get_count(NXT_PORT_C),
		 		(F32)nxt_motor_get_count(NXT_PORT_B),
				(F32)ecrobot_get_battery_voltage(),
				&pwm_l,
				&pwm_r);
			nxt_motor_set_speed(NXT_PORT_C, pwm_l, 1);
			nxt_motor_set_speed(NXT_PORT_B, pwm_r, 1);
			break;

		case (RN_MODE_BALANCEOFF):
			nxt_motor_set_speed(NXT_PORT_C, pwm_l, 1);
			nxt_motor_set_speed(NXT_PORT_B, pwm_r, 1);
			break;

		default:
			break;
	}
}

/*
 *	�e��^�X�N
 */

//���s���@�Ǘ�(4ms)
TASK(ActionTask)
{
	RN_modesetting();	//���s�̏��
	tailcontrol();		//�K���R���g���[��
	TerminateTask();
}

//���s��ԊǗ�(5ms)
TASK(ActionTask2)
{
	RN_setting();		//���s���
	TerminateTask();
}

//���O���M�Ǘ�(50ms)
TASK(LogTask)
{
	logSend(cmd_forward,cmd_turn,pwm_l,pwm_r,0,0);		//���O���
	TerminateTask();
}


/******************************** END OF FILE ********************************/

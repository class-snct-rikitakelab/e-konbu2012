#include "sample_PID.h"
#include "math.h"

/*
 *	�e��ϐ���`
 */

//���C���g���[�X�p�ڕW�l
static unsigned int BLACK_VALUE;	//���l
static unsigned int WHITE_VALUE;	//���l
static unsigned int GRAY_VALUE;		//�D�F�l�i���݂͍��Ɣ��̕��ϒl�j

//�W���C���I�t�Z�b�g�l
static unsigned int GYRO_OFFSET;	

static int counter = 0;

//�K���ݒ�p�x
#define ANGLEOFDOWN 95			//�~���ڕW�p�x
#define ANGLEOFUP 0				//�㏸�ڕW�p�x

//���x���ߌW��
#define SPEED_COUNT 20

//PID����p�΍��l
static float hensa;					//P����p
static float i_hensa = 0;			//I����p
static float d_hensa = 0;			//D����p
static float bf_hensa = 0;

//���C���g���[�X��PID����p�W��
static float Kp = 1.360;			//P����p
static float Ki = 2.6;				//I����p
static float Kd = 0.003;			//D����p

//�W���C���Z���T�I�t�Z�b�g�v�Z�p�ϐ�
static U32	gyro_offset = 0;    /* gyro sensor offset value */ 

//Bluetooth�ʐM�p�o�b�t�@
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
	RN_SETTINGMODE_START,		//�������
	RN_RUN,						//��{���s�i���C���g���[�X�j
	RN_STOP,					//��~
} RN_SETTINGMODE;


//�K���̏��
typedef enum{
	RN_TAILDOWN,				//�K���~��
	RN_TAILUP,					//�K���㏸
} RN_TAILMODE;


//�������
RN_MODE runner_mode = RN_MODE_INIT;
RN_SETTINGMODE setting_mode = RN_SETTINGMODE_START;
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
void RA_speed(int limit,int s_Kp);
void RN_modesetting();
static int remote_start(void);

//�J�E���^�̐錾
DeclareCounter(SysTimerCnt);

//�t���f�B�X�v���C�ɕ\������V�X�e�����ݒ�
const char target_subsystem_name[] = "sample";


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


	RA_speed(forward_speed,2);	//���x��i�K�I�ɕω�

	if(forward_speed > 0)
		hensa = (float)GRAY_VALUE - (float)ecrobot_get_light_sensor(NXT_PORT_S3);
	else
		hensa = (float)ecrobot_get_light_sensor(NXT_PORT_S3) - (float)GRAY_VALUE;

	i_hensa = i_hensa+(hensa*0.0005);
	d_hensa = (hensa - bf_hensa)/0.0005;
	bf_hensa = hensa;

	cmd_turn = -(Kp * hensa + Ki * i_hensa + Kd * d_hensa);

	if (-100 > cmd_turn) {
		cmd_turn = -100;
	} else if (100 < cmd_turn) {
		cmd_turn = 100;
	}

	/*�|������OFF��*/
	nxt_motor_set_speed(NXT_PORT_C, forward_speed + cmd_turn/2, 1);
	nxt_motor_set_speed(NXT_PORT_B, forward_speed - cmd_turn/2, 1);

}

//�i�K�I�����p�֐��i�w��ʂ������x�����X�ɏ㏸�j
void RA_speed(int limit,int s_Kp){

	static int forward_speed;

	counter ++;

	if(counter >= SPEED_COUNT)
	{

		forward_speed = cmd_forward;

		if(limit-forward_speed >= 0){
			forward_speed += s_Kp;

			if(forward_speed > limit)
				forward_speed = limit;
		}
		else{
			forward_speed -= s_Kp;

			if(forward_speed < limit)
				forward_speed = limit;
		}

		cmd_forward = forward_speed;
		counter =0;


	}
}

//�K���p�x�R���g���[���֐�
void tailcontrol(){

	static const float t_Kp = 1.7;

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
	static int speed = 30;   //���x
	
	switch (setting_mode){

		//���s�J�n�O
		case (RN_SETTINGMODE_START):
			RN_calibrate();				//�L�����u���[�V����
			break;

		//�ʏ푖�s
		case (RN_RUN):
			RA_linetrace_PID(speed);	//PID����ɂ�郉�C���g���[�X
			break;

		default:
			break;

		}
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

	//�W���C���I�t�Z�b�g�y�уo�b�e���d���l
	while(1){
		if(ecrobot_get_touch_sensor(NXT_PORT_S4) == TRUE)
		{
			ecrobot_sound_tone(932, 512, 10);
			gyro_offset += (U32)ecrobot_get_gyro_sensor(NXT_PORT_S1);
			GYRO_OFFSET = gyro_offset;
			systick_wait_ms(500);
			break;
		}
	}

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

//��ԕ\���Ǘ�(20ms)
TASK(DisplayTask)
{
	TerminateTask();
}

//���O���M�Ǘ�(50ms)
TASK(LogTask)
{
	TerminateTask();
}


/******************************** END OF FILE ********************************/
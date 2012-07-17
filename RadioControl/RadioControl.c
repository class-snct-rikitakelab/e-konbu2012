/*
 * RadioControl.c
 * ET�o�g���v���O����
 */

#include "RadioControl.h"

/*
 *	�e���`
*/

//�K���ݒ�p�x
#define ANGLEOFDOWN 105 				//�~���ڕW�p�x
#define ANGLEOFUP 0						//�㏸�ڕW�p�x

/*
 *	�O���[�o���ϐ�
 */

//�K��PI����p�W��
static float t_Kp = 3.85;			//P����p

//�K������p�ϐ�
static int t_angle = 0;				//���݂̊p�x
static int t_count = 0;				//���䑬�x���ߗp�J�E���^
static int t_value = 0;				//�p�x�ڕW�l
static int t_count_limit = 0;		//�J�E���^�ő�l
static int t_up = 0;				//�����l

//�S�̗p�J�E���^�i���ԉ҂��p�j
static int wait_count = 0;

/* �o�����X�R���g���[���֓n���R�}���h�p�ϐ� */
S8  cmd_forward, cmd_turn;
/* �o�����X�R���g���[������Ԃ���郂�[�^����p�ϐ� */
S8	pwm_l, pwm_r;

static U32	gyro_offset = 0;    /* gyro sensor offset value */

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
	RN_START,					//�J�n�҂����[�h
	RN_RUN,						//�ΐ탂�[�h
	RN_PUSHBUTTON				//�{�^���������[�h
} RN_SETTINGMODE;

//�K���̏��
typedef enum{
	RN_TAILDOWN,				//�K���~��
	RN_TAILUP,					//�K���㏸
} RN_TAILMODE;

//�������
RN_MODE runner_mode = RN_MODE_INIT;
RN_SETTINGMODE setting_mode = RN_START;
RN_TAILMODE tail_mode = RN_TAILUP;

/*
 *	�e��v���C�x�[�g�֐���`
 */


//�e��v���C�x�[�g�֐�
void RN_calibrate();
void RN_setting();
void RN_modesetting();
void runner_mode_change(int flag);
void tailpower(float value);
void tail_mode_change(int mode,int value,int limit,int t_up);
void taildown();

#define BT_RCV_BUF_SIZE (32) /* it must be 32bytes with NXT GamePad */
static U8 bt_receive_buf[BT_RCV_BUF_SIZE]; /* Bluetooth receive buffer(32bytes) */

//�J�E���^�̐錾
DeclareCounter(SysTimerCnt);

//�t���f�B�X�v���C�ɕ\������V�X�e�����ݒ�
const char target_subsystem_name[] = "RadioControl";

/*
 *	�֐�����
 */


//���������֐��i�v���O�����̍ŏ��ɌĂяo���j
void ecrobot_device_initialize(void)
{
	ecrobot_set_light_sensor_active(NXT_PORT_S3);	//���Z���T�N��
	ecrobot_init_bt_slave("LEJOS-OSEK");			//Bluetooth�N��

	//���[�^���Z�b�g
	ecrobot_set_motor_rev(NXT_PORT_A,0);
	ecrobot_set_motor_rev(NXT_PORT_B,0);
	ecrobot_set_motor_rev(NXT_PORT_C,0);
	ecrobot_set_motor_speed(NXT_PORT_A,0);
	ecrobot_set_motor_speed(NXT_PORT_B,0);
	ecrobot_set_motor_speed(NXT_PORT_C,0);
}



//��n�������֐��i�v���O�����I�����Ăяo���j
void ecrobot_device_terminate(void)
{
	tail_mode = RN_TAILUP;							//�K�����グ��

	ecrobot_set_light_sensor_inactive(NXT_PORT_S3);	//���Z���T�I��
	ecrobot_term_bt_connection();					//Bluetooth�I��

	//���[�^���Z�b�g
	ecrobot_set_motor_rev(NXT_PORT_A,0);
	ecrobot_set_motor_rev(NXT_PORT_B,0);
	ecrobot_set_motor_rev(NXT_PORT_C,0);
	ecrobot_set_motor_speed(NXT_PORT_A, 0);			
	ecrobot_set_motor_speed(NXT_PORT_B, 0);
	ecrobot_set_motor_speed(NXT_PORT_C, 0);
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
	ercd = SignalCounter( SysTimerCnt );
	if( ercd != E_OK ){
		ShutdownOS( ercd );
	}
}

//�K���p�x���ߊ֐�
void taildown(){

	//X-ecrobot_get_motor_rev(NXT_PORT_S4) ��X = �ڕW�l
	//�ڕW�l��1������(���s�ڕW�lt_angle���̂��̂��ŏI�ڕW�lt_value�֋߂Â��Ă���)
	//t_angle�̏����l�͌��݂̊p�x

	static float t_hensa = 0;	//�K���p�x�̖ڕW�l�Ƃ̍�
	static float t_speed = 0;	//�K�����[�^�ɑ��鑬�x

	t_count++;					//���x����p�J�E���^

	switch(tail_mode){
		case(RN_TAILDOWN):				//�K����������
			if(t_angle <= t_value)		//���݂̊p�x���ڕW�l�ȉ����ǂ���
			{
				t_hensa = t_angle - ecrobot_get_motor_rev(NXT_PORT_A);
				if(t_count >= t_count_limit)	//�J�E���^�ŐK���������鑬�x�𒲐�
				{
					t_angle+=t_up;			//�p�x���グ��
					t_count = 0;
				}
			}
			else
			{
				t_angle = t_value;
			}

			break;

		case(RN_TAILUP):				//�K�����グ��
			if(t_angle >= t_value)		//���݂̊p�x���ڕW�l�ȏォ�ǂ���
			{
				t_hensa = t_angle - ecrobot_get_motor_rev(NXT_PORT_A);
				if(t_count >= t_count_limit)	//�J�E���^�ŐK�����グ�鑬�x�𒲐�
				{
					t_angle-=t_up;			//�p�x��������
					t_count = 0;
				}
			}
			else
			{
				t_angle = t_value;
			}

			break;

		default:
			break;

	}

	t_speed = t_Kp*t_hensa;	//�΍������ɑ��x����

	if (t_speed < -100)
		t_speed = -100;
	else if (t_speed > 100)
		t_speed = 100;

	ecrobot_set_motor_speed(NXT_PORT_A, t_speed);	//���[�^�ɑ��x�𑗂�

}

//�K�����[�h�ύX�֐�
void tail_mode_change(int mode,int value,int limit,int up)	//mode(0:�K�������낷�A1:�K�����グ��) value(�ڕW�l) limit(�グ��قǒx���j up�i�グ��قǉ����j1
{

	static int flag;					//�K�����[�h�t���O

	switch(mode){
		case 0:
			tail_mode = RN_TAILDOWN;	//�K�������낷
			flag = 0;
			break;
		case 1:
			tail_mode = RN_TAILUP;		//�K�����グ��
			flag = 1;
			break;
		default:
			break;
	}

	//�ڕW�l���ς�������̂݁A�p�x�����߂�i������Ăяo���ꍇ�������l���L�[�v���邽�߁j
	if(t_value != value)
		t_angle = ecrobot_get_motor_rev(NXT_PORT_A);

	t_value = value;			//�ڕW�l�ݒ�
	t_count_limit = limit;		//�J�E���^�ő�l�ݒ�
	t_up = up;					//�p�x�����l�ݒ�

}

//���s��Ԑݒ�֐�
void RN_setting()
{

	switch (setting_mode){

			//�L�����u���[�V�������
		case (RN_START):
			RN_calibrate();
			break;
		
			//�ʏ푖�s���
		case (RN_RUN):
			(void)ecrobot_read_bt_packet(bt_receive_buf, BT_RCV_BUF_SIZE);
			
			cmd_forward = -((S8)bt_receive_buf[0]/2);
			cmd_turn = ((S8)bt_receive_buf[1]/2);
			
			nxt_motor_set_speed(NXT_PORT_C, cmd_forward + cmd_turn/2, 1);
			nxt_motor_set_speed(NXT_PORT_B, cmd_forward - cmd_turn/2, 1);
		
			if(ecrobot_get_touch_sensor(NXT_PORT_S4) == 1)
			{
				ecrobot_sound_tone(980,512,30);
				nxt_motor_set_speed(NXT_PORT_C,100,1);
				nxt_motor_set_speed(NXT_PORT_B,-100,1);
				wait_count = 0;
				setting_mode = RN_PUSHBUTTON;
			}
			
			break;

			//���b�N�A�b�v�Q�[�g���s�������
		case (RN_PUSHBUTTON):
			wait_count++;

			if(wait_count >= 150)
			{
				tail_mode_change(1,ANGLEOFUP,1,1);
				nxt_motor_set_speed(NXT_PORT_C,0,1);
				nxt_motor_set_speed(NXT_PORT_B,0,1);
			}

			if(wait_count == 1000)
				setting_mode = RN_START;

			break;

		default:
			break;
	}
}

//�L�����u���[�V�����֐�
void RN_calibrate()
{
	/*�o�����T�[ON�p
	while(1){
		if(ecrobot_get_touch_sensor(NXT_PORT_S4) == TRUE)
		{
			ecrobot_sound_tone(932, 512, 10);
			gyro_offset += (U32)ecrobot_get_gyro_sensor(NXT_PORT_S1);
			systick_wait_ms(500);
			break;
		}
	}
	*/
	while(1){
		if(ecrobot_get_touch_sensor(NXT_PORT_S4) == TRUE)
		{
			ecrobot_sound_tone(982,512,10);
			tail_mode_change(0,ANGLEOFDOWN,1,2);
			setting_mode = RN_RUN;
			runner_mode = RN_MODE_BALANCEOFF;
//			runner_mode = RN_MODE_BALANCE;
			systick_wait_ms(500);
			break;
		}
	}

}

//���s�̏�Ԑݒ�֐�
void RN_modesetting()
{
	switch (runner_mode){
			//���s�̏������
		case (RN_MODE_INIT):
			cmd_forward = 0;
			cmd_turn = 0;
			break;

			//�o�����T�[
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

			//�o�����T�[����
		case (RN_MODE_BALANCEOFF):
			break;

		default:
			nxt_motor_set_speed(NXT_PORT_C, 0, 1);
			nxt_motor_set_speed(NXT_PORT_B, 0, 1);
			break;
	}
}


/*
 *	�e��^�X�N
 */

//���s�̊Ǘ��^�X�N(4ms)
TASK(ActionTask)
{
	RN_modesetting();	//���s�̏�Ԑݒ�
	taildown();			//�K������
	TerminateTask();
}

//���s��ԊǗ��^�X�N(5ms)
TASK(ActionTask2)
{
	RN_setting();		//���s��Ԑݒ�
	TerminateTask();
}

//��ԕ\���Ǘ��^�X�N(20ms)
TASK(DisplayTask)
{
	ecrobot_status_monitor(target_subsystem_name);	//���j�^�ɏ�ԕ\��
	TerminateTask();
}

/******************************** END OF FILE ********************************/

/*
 * RadioControl.c
 * ET�o�g���v���O����
 */

#include "ETBattle.h"

/*
 *	�e���`
*/

/*
 *	�O���[�o���ϐ�
 */

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
	RN_BOOST,					//�^�[�{
	RN_PUSHBUTTON				//�{�^���������[�h
} RN_SETTINGMODE;


typedef enum{
	BC_ONE,
	BC_TWO,
	BC_THREE,
	BC_FOUR,
} BC_MODE;

//�������
RN_MODE runner_mode = RN_MODE_INIT;
RN_SETTINGMODE setting_mode = RN_START;
BC_MODE boostCheckMode = BC_ONE;

/*
 *	�e��v���C�x�[�g�֐���`
 */

//�e��v���C�x�[�g�֐�
void RN_calibrate();
void RN_setting();
void RN_modesetting();
void runner_mode_change(int flag);
int boost();

#define BT_RCV_BUF_SIZE (32) /* it must be 32bytes with NXT GamePad */
static U8 bt_receive_buf[BT_RCV_BUF_SIZE]; /* Bluetooth receive buffer(32bytes) */

//�J�E���^�̐錾
DeclareCounter(SysTimerCnt);

//�t���f�B�X�v���C�ɕ\������V�X�e�����ݒ�
const char target_subsystem_name[] = "ETBattle";

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

//���s��Ԑݒ�֐�
void RN_setting()
{

	static int wait_count = 0;

	switch (setting_mode){

			//�L�����u���[�V�������
		case (RN_START):
			RN_calibrate();
			break;
		
			//�ΐ풆
		case (RN_RUN):
			(void)ecrobot_read_bt_packet(bt_receive_buf, BT_RCV_BUF_SIZE);	/* �X�e�B�b�N���� */
			
			cmd_forward = -((S8)bt_receive_buf[0])/2;	/* �O�i��(���̂܂܂ł͑�������̂Œl�𔼕��j */
			cmd_turn = ((S8)bt_receive_buf[1]);			/* ����� */
			
			//�^�[�{�`�F�b�N
			if(boost() == 1)
			{
				setting_mode = RN_BOOST;
				ecrobot_sound_tone(980,512,100);
			}

			else /* �^�[�{�����A�X�e�B�b�N���� */
			{
				nxt_motor_set_speed(NXT_PORT_C, cmd_forward + cmd_turn/2, 1);
				nxt_motor_set_speed(NXT_PORT_B, cmd_forward - cmd_turn/2, 1);
			}

			if(ecrobot_get_touch_sensor(NXT_PORT_S4) == 1)	/* �q�b�g�`�F�b�N */
			{
				ecrobot_sound_tone(980,512,100);
				nxt_motor_set_speed(NXT_PORT_C,100,1);
				nxt_motor_set_speed(NXT_PORT_B,-100,1);
				wait_count = 0;
				setting_mode = RN_PUSHBUTTON;
			}
			
			break;

			//�^�[�{����(ver 2.0�V�@�\�j
		case (RN_BOOST):
			wait_count++;

			/* ���ԗւɃ��[�^����ʂ̍ő�l�𑗐M�i�X�e�B�b�N����s�j */
			nxt_motor_set_speed(NXT_PORT_C,127,1);
			nxt_motor_set_speed(NXT_PORT_B,127,1);

			/* 1�b��ɒʏ탂�[�h�ɕ��A */
			if(wait_count > 125)
			{
				setting_mode = RN_RUN;
				wait_count = 0;
			}
			break;

			//�s�k����
		case (RN_PUSHBUTTON):
			wait_count++;

			if(wait_count >= 150)
			{
				TailAngleChange(ANGLEOFZERO);
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


/*
 *	�^�[�{���͊֐�
 *	�f�������X�e�B�b�N��2���ɓ��͂��邱�ƂŃ^�[�{�i�e���͂̊Ԋu��0.2�b�ȓ��j
 *	����:�����A�Ԃ�l:�^�[�{�t���O
 */
int boost(){

	static int counter = 0;
	int boostflag = 0;

	switch(boostCheckMode){
		case (BC_ONE):	/* �X�e�B�b�N�������i1��ځj */
			if(cmd_forward == 0)
			{
				boostCheckMode = BC_TWO;
				counter = 0;
			}
			break;
		case(BC_TWO):	/* �X�e�B�b�N���ŏ�i1��ځj */
			if(cmd_forward == 50)
			{
				boostCheckMode = BC_THREE;
//				ecrobot_sound_tone(982,512,10);
				counter = 0;
			}
			else if(counter++ > BOOSTTIME/20)
				boostCheckMode = BC_ONE;
			break;
		case (BC_THREE):	/* �X�e�B�b�N�������i2��ځj */
			if(cmd_forward == 0)
			{
				boostCheckMode = BC_FOUR;
//				ecrobot_sound_tone(982,512,10);
				counter = 0;
			}
			else if(counter++ > BOOSTTIME/20)
				boostCheckMode = BC_ONE;
			break;
		case (BC_FOUR):		/* �X�e�B�b�N���ŏ�i2��ځj�i�����Ń^�[�{�t���OON�j */
			if(cmd_forward == 50)
			{
				boostflag = 1;
				boostCheckMode = BC_ONE;
//				ecrobot_sound_tone(982,512,10);
				counter = 0;
			}
			else if(counter++ > BOOSTTIME/20)
				boostCheckMode = BC_ONE;
			break;
		default:
			break;
	}

	return boostflag;	//�^�[�{�t���O�i0:�����A1:�^�[�{ON�j
}

//�L�����u���[�V�����֐�
void RN_calibrate()
{
	/*�o�����T�[ON�p*/
	/*
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
			TailAngleChange(ANGLEOFDOWN);
			setting_mode = RN_RUN;
			runner_mode = RN_MODE_BALANCEOFF;
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
	TailControl();
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

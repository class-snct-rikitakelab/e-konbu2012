/*
 * MainRunningInCourse.c
 * �C���R�[�X���s�v���O����
 */


#include "MainRunningInCourse.h"

/*
 *	�e���`
*/

//���x�J�E���^�̏���l
#define SPEED_COUNT 10


/*
 *	�O���[�o���ϐ�
 */



//���x���߃J�E���^�i�J�E���^������ɒB���閈�ɑ��x��1�㏸
static int speed_counter = 0;

//�K������p�ϐ�
static int t_angle = 0;				//���݂̊p�x
static int t_count = 0;				//���䑬�x���ߗp�J�E���^
static int t_value = 0;				//�p�x�ڕW�l
static int t_count_limit = 0;		//�J�E���^�ő�l
static int t_up = 0;				//�����l

//�S�̗p�J�E���^�i���ԉ҂��p�j
static int time_counter = 0;

//�����g�Z���T�ڕW�l
static int target_sonar = 20;

//�����g�Z���T�p�t���O
static int sonarflag;

static int sonarvalue;

//�W���C���Z���T�I�t�Z�b�g�v�Z�p�ϐ�
static U32	gyro_offset = 0;    /* gyro sensor offset value */

char rx_buf[BT_MAX_RX_BUF_SIZE];

/* �o�����X�R���g���[���֓n���R�}���h�p�ϐ� */
S8  cmd_forward, cmd_turn;
/* �o�����X�R���g���[������Ԃ���郂�[�^����p�ϐ� */
S8	pwm_l, pwm_r;


//�����v���p�ϐ�
int revL = 0;
int revR = 0;

int distance_before_gate;	//���b�N�A�b�v�Q�[�g�ʉߑO����
int distance_after_gate;	//���b�N�A�b�v�Q�[�g�ʉߒ�����


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
	RN_LOOKUP_START,
	RN_LOOKUP_STOP,					//���b�N�A�b�v�Q�[�g����
	RN_LOOKUP_DOWN,				//���s�̍~��
	RN_LOOKUP_MOVE,				//���s�̑O�i
	RN_LOOKUP_UP					//���s�̕��A
} RN_SETTINGMODE;



//�������
RN_MODE runner_mode = RN_MODE_INIT;
RN_SETTINGMODE setting_mode = RN_SETTINGMODE_START;
RN_TAILMODE tail_mode = RN_TAILUP;

/*
 *	�e��v���C�x�[�g�֐���`
 */


//�e��v���C�x�[�g�֐�
void RN_setting();
int online();
void RA_linetrace(int forward_speed, int turn_speed);
void RA_linetrace_PID(int forward_speed);
void logSend(S8 data1, S8 data2, S16 adc1, S16 adc2, S16 adc3, S16 adc4);
void taildown();
void RA_linetrace_P(int forward_speed);
void RA_speed(int limit,int s_Kp);
int RA_wheels(int turn);
void RN_modesetting();
static int remote_start(void);
int sonarcheck(int target_sonar);
void runner_mode_change(int flag);
void tailpower(float value);
void tail_mode_change(int mode,int value,int limit,int t_up);
void RA_linetrace_PID_balanceoff(int forward_speed);
void getsonarvalue(void);

void RA_hensareset(void);


void self_location(void);
unsigned char MKTrigger(void);
signed char LVTrigger(void);
int abs(int n);

//�J�E���^�̐錾
DeclareCounter(SysTimerCnt);

//�t���f�B�X�v���C�ɕ\������V�X�e�����ݒ�
const char target_subsystem_name[] = "InCourse";



/*
 *	�֐�����
 */


//���������֐��i�v���O�����̍ŏ��ɌĂяo���j
void ecrobot_device_initialize(void)
{
	ecrobot_set_light_sensor_active(NXT_PORT_S3);	//���Z���T�N��
	ecrobot_init_bt_slave("LEJOS-OSEK");			//Bluetooth�N��
	ecrobot_init_sonar_sensor(NXT_PORT_S2);			//�����g�Z���T�N��

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
	tail_mode = RN_TAILUP;							//�K�����グ��i�Ӗ�����̂��H�j

	ecrobot_set_light_sensor_inactive(NXT_PORT_S3);	//���Z���T�I��
	ecrobot_term_sonar_sensor(NXT_PORT_S2);			//�����g�Z���T�I��
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
	/*
	 *  Increment OSEK Alarm System Timer Count
     */
	ercd = SignalCounter( SysTimerCnt );
	if( ercd != E_OK ){
		ShutdownOS( ercd );
	}
}

//2�ԗւ̉�]�ʍ��AP����֐��i�ڕW�l�ɋ߂Â���j
int RA_wheels(int turn){
	float w_kp = 1.4;	//��]��P����p�����[�^

	signed long def = ecrobot_get_motor_rev(NXT_PORT_B) - ecrobot_get_motor_rev(NXT_PORT_C);	//��]�ʍ�
	turn = w_kp * def;	//��]�ʎZ�o

	return turn;
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

void RA_hensareset(void)
{
	hensa = 0;
	i_hensa = 0;
	d_hensa = 0;
	bf_hensa = 0;
}

//�����g�Z���T��Ԍ��o�֐�
int sonarcheck(int target_sonar)
{
	if(sonarvalue <= target_sonar)	//�����g�Z���T�̒l���ڕW�l�ȉ������f���t���O�ύX
	{
		return 1;
	}
	else
		return 0;
}

void getsonarvalue(void)
{
	sonarvalue = ecrobot_get_sonar_sensor(NXT_PORT_S2);
}



//�����[�g�X�^�[�g�Ǘ��֐�
static int remote_start(void)
{
	int i;
	unsigned int rx_len;
	unsigned char start = 0;		//��ԃt���O

	for (i=0; i<BT_MAX_RX_BUF_SIZE; i++)
	{
		rx_buf[i] = 0; //��M�o�b�t�@���N���A
	}

	rx_len = ecrobot_read_bt(rx_buf, 0, BT_MAX_RX_BUF_SIZE);
	if (rx_len > 0)
	{
		//��M�f�[�^����
		if (rx_buf[0] == CMD_START)
		{
			start = 1; //���s�J�n�t���O
		}
		ecrobot_send_bt(rx_buf, 0, rx_len); //��M�f�[�^���G�R�[�o�b�N
	}

	return start;
}

//���s��Ԑݒ�֐�
void RN_setting()
{

	switch (setting_mode){

			//�L�����u���[�V�������
		case (RN_SETTINGMODE_START):
			RN_calibrate();
			time_counter = 0;
			break;

		case (RN_SPEEDZERO):
			cmd_forward = 0;
			cmd_turn = RA_wheels(cmd_turn);
			time_counter++;
			if(time_counter >= 100)
			{
				setting_mode = RN_RUN;
				time_counter = 0;
			}
			break;
		
			//�ʏ푖�s���
		case (RN_RUN):
			RA_linetrace_PID_balanceoff(25);
			
			setting_mode = RN_LOOKUP_START;
			
			break;

		case (RN_LOOKUP_START):
			RA_linetrace_PID_balanceoff(25);

			time_counter++;

			if(time_counter >= 150)					//�X�^�[�g���ɔ�������̂�h���i�e�X�g�p�j
			{
				if(sonarcheck(19) == 1)				//�����g�Z���T�������������ǂ���
				{
					ecrobot_sound_tone(900,512,30);
					setting_mode = RN_LOOKUP_STOP;
					time_counter = 0;
				}
			}
			break;

			//���b�N�A�b�v�Q�[�g���s�������
		case (RN_LOOKUP_STOP):
			RA_linetrace_PID_balanceoff(5);
			//cmd_turn = RA_wheels(cmd_turn);

			time_counter++;

			if(time_counter == 200)
			{
				//tail_mode_change(0,ANGLEOFDOWN,2,1);
				
				while(time_counter <= 1200 || t_angle <= ANGLEOFDOWN)
				{
					RA_speed(-20,5);
					cmd_turn = RA_wheels(cmd_turn);
					time_counter++;
				}
				setting_mode = RN_LOOKUP_DOWN;
				time_counter = 0;
				runner_mode_change(2);
			}

			break;
		
			//���b�N�A�b�v�Q�[�g���s�A�K���~��
		case (RN_LOOKUP_DOWN):
			nxt_motor_set_speed(NXT_PORT_C, 0, 1);
			nxt_motor_set_speed(NXT_PORT_B, 0, 1);

			time_counter++;

			if(time_counter >= 200)
			{
				tailpower(15.0);
				tail_mode_change(1,ANGLEOFLOOKUP,4,1);
				if(ecrobot_get_motor_rev(NXT_PORT_A) == ANGLEOFLOOKUP)
					{
						setting_mode = RN_LOOKUP_MOVE;
						time_counter = 0;
						revL = nxt_motor_get_count(NXT_PORT_C);
						revR = nxt_motor_get_count(NXT_PORT_B);
						distance_before_gate = fabs(CIRCUMFERENCE/360.0 * ((revL+revR)/2.0));
					}
			}

			break;

			//���b�N�A�b�v�Q�[�g���s�A�K���~����ԂőO�i
		case (RN_LOOKUP_MOVE):

//			RA_linetrace(30,20);
			RA_linetrace_PID_balanceoff(25);
			revL = nxt_motor_get_count(NXT_PORT_C);
			revR = nxt_motor_get_count(NXT_PORT_B);
			distance_after_gate = fabs(CIRCUMFERENCE/360.0 * ((revL+revR)/2.0));
			
			if(distance_after_gate - distance_before_gate > 30)
			{	
				setting_mode = RN_LOOKUP_UP;
			}
			
			break;

			//���b�N�A�b�v�Q�[�g���s�A�O�i��|����Ԃ֕��A
		case (RN_LOOKUP_UP):
			if(time_counter < 200)
			{
				nxt_motor_set_speed(NXT_PORT_C, 0, 1);
				nxt_motor_set_speed(NXT_PORT_B, 0, 1);
			}
			time_counter++;

			//�K�������グ�⏕
			if(time_counter == 200)
			{
				tail_mode_change(0,ANGLEOFDOWN,4,1);
				
				ecrobot_set_motor_speed(NXT_PORT_B, -15);	//���[�^�ɑ��x�𑗂�
				ecrobot_set_motor_speed(NXT_PORT_C, -15);	//���[�^�ɑ��x�𑗂�
			}

			if(t_angle == ANGLEOFDOWN)
			{
				ecrobot_set_motor_speed(NXT_PORT_B, 0);	//���[�^�ɑ��x�𑗂�
				ecrobot_set_motor_speed(NXT_PORT_C, 0);	//���[�^�ɑ��x�𑗂�	
			}
			/*
			if(t_angle == ANGLEOFDOWN && time_counter >= 1000)
			{
				tail_mode_change(1,ANGLEOFPUSH,0,10);
				nxt_motor_set_speed(NXT_PORT_C, 0, 1);
				nxt_motor_set_speed(NXT_PORT_B, 0, 1);
			}
			*/
			if(t_angle == ANGLEOFDOWN && time_counter >= 1050)
			{
				tailpower(1.85);			

				//tail_mode_change(1,ANGLEOFUP,0,2);

				ecrobot_set_motor_rev(NXT_PORT_B,0);
				ecrobot_set_motor_rev(NXT_PORT_C,0);
				ecrobot_set_motor_speed(NXT_PORT_B,0);
				ecrobot_set_motor_speed(NXT_PORT_C,0);
				/*
				runner_mode_change(1);
				RA_hensareset();
				balance_init();
				time_counter=0;
				cmd_forward=0;
				*/
				setting_mode = RN_LOOKUP_START;
			}
			
			break;

		default:
			break;
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
	self_location();	//���Ȉʒu����
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

//���O���M�A�����g�Z���T�Ǘ��^�X�N(50ms) (����50ms�łȂ���Γ��삵�Ȃ��j
TASK(LogTask)
{
	logSend(cmd_forward,cmd_turn,BLACK_VALUE,0,		//Bluetooth��p���ăf�[�^���M
			x_r,y_r);

	getsonarvalue();

	TerminateTask();
}



/******************************** END OF FILE ********************************/

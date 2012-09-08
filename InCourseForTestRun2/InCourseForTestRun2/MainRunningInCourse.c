/*
 * lookupgate.c
 * ���b�N�A�b�v�Q�[�g�ʉ߃v���O����
 */


#include "MainRunningInCourse.h"
#include "math.h"


//�W���C���U���l
#define PM_GYRO 65

//�ԗւ̉~��[cm]
#define CIRCUMFERENCE 25.8			//�ԗւ̉~��


/* �ԗ֔��a�A���s�̕�*/
#define WHEEL_R		41	//[mm]
#define MACHINE_W	162	//[mm]

/*
 *	�O���[�o���ϐ�
 */


//�S�̗p�J�E���^�i���ԉ҂��p�j
static int wait_count = 0;

//�����g�Z���T�ڕW�l
static int target_sonar = 20;

//�����g�Z���T�p�t���O
static int sonarflag;

static int sonarvalue;

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

/*
 *	�e��v���C�x�[�g�֐���`
 */


//�e��v���C�x�[�g�֐�
void RN_setting();
void logSend(S8 data1, S8 data2, S16 adc1, S16 adc2, S16 adc3, S16 adc4);
int RA_wheels(int turn);
void RN_modesetting();
int sonarcheck(int target_sonar);
void runner_mode_change(int flag);
void getsonarvalue(void);

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

//���s�̃��[�h�ύX�֐��i��Ƀo�����T�[��ON/OFF�j
void runner_mode_change(int flag)
{
	switch(flag){
	case 0:
		runner_mode = RN_MODE_INIT;			//���s�̏������
		break;
	case 1:
		runner_mode = RN_MODE_BALANCE;		//�o�����T�[ON
		break;
	case 2:
		runner_mode = RN_MODE_BALANCEOFF;	//�o�����T�[OFF
		break;
	default:
		break;
	}
}

//���s��Ԑݒ�֐�
void RN_setting()
{

	switch (setting_mode){

			//�L�����u���[�V�������
		case (RN_SETTINGMODE_START):
			if(RN_calibrate() == 1)
			{
				setting_mode = RN_RUN;
				TailAngleChange(ANGLEOFDOWN);
			}
			break;
		
			//�ʏ푖�s���
		case (RN_RUN):
			RA_linetrace_PID(RA_speed(25));
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
		 		(F32)getGyroOffset(),
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
	TailControl();			//�K������
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
	logSend(cmd_forward,cmd_turn,dist,theta,x_r,y_r);			//Bluetooth��p���ăf�[�^���M

	getsonarvalue();

	TerminateTask();
}



/******************************** END OF FILE ********************************/

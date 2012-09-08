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

/*
//�V�X�e���S�̂̏��
typedef enum{
	RN_MODE_INIT, 					//�������
	RN_MODE_BALANCE,				//�|������ON
	RN_MODE_TAIL,				//�|������OF
	RN_MODE_STOP
} RN_MODE;
*/

//�L�����u���[�V�����̏��
typedef enum{
	RN_SETTINGMODE_START,		//�������
	RN_RUN,						//��{���s�i���C���g���[�X�j
	RN_SLOPE,					//�⓹
	RN_RUN_SECOND,				//�⓹��̊�{���s
	RN_LOOKUPGATE,				//���b�N�A�b�v�Q�[�g
	RN_RUN_THIRD,				//���b�N�A�b�v�Q�[�g��̊�{���s
	RN_DRIFTTURN,				//�h���t�g�^�[��
} RN_SETTINGMODE;

//�������
RN_SETTINGMODE setting_mode = RN_SETTINGMODE_START;

/*
 *	�e��v���C�x�[�g�֐���`
 */

//�e��v���C�x�[�g�֐�
void RN_setting();
int RA_wheels(int turn);
int sonarcheck(int target_sonar);
void getsonarvalue(void);

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

//���s��Ԑݒ�֐�
void RN_setting()
{

	switch (setting_mode){

			//�L�����u���[�V�������
		case (RN_SETTINGMODE_START):
			if(RN_calibrate() == 1)
			{
				setting_mode = RN_RUN;
				PWMGeneratorModeChange(RN_MODE_TAIL);
				TailAngleChange(ANGLEOFDOWN);
			}
			break;
		
			//�ʏ푖�s���
		case (RN_RUN):
			setCmdForward(RA_speed(40));
			setCmdTurn(RA_linetrace_PID(getCmdForward()));
			if(getInitGyroOffset() - 30 > (U32)ecrobot_get_gyro_sensor(NXT_PORT_S1) && wait_count > 500)
			{
				ecrobot_sound_tone(880, 512, 30);
				setting_mode = RN_SLOPE;
				wait_count = 0;
			}
			break;
			
		case (RN_SLOPE):
			if(runningSlope() == 1)
				setting_mode = RN_RUN_SECOND;
			break;
			
		case (RN_RUN_SECOND):
			setCmdForward(RA_speed(40));
			setCmdTurn(RA_linetrace_PID(getCmdForward()));
			break;

		case (RN_LOOKUPGATE):
			break;
			/*
		case (RN_RUN_THIRD):
			break;

		case (RN_DRIFTTURN):
			break;
			*/
		default:
			break;
	}
}

/*
 *	�e��^�X�N
 */

//���s�̊Ǘ��^�X�N(4ms)
TASK(ActionTask)
{
	//RN_modesetting();	//���s�̏�Ԑݒ�
	calcPWMValues();
	TailControl();			//�K������
	selflocation();			//���Ȉʒu����
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

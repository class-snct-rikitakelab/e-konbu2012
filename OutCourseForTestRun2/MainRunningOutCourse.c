/*
 * lookupgate.c
 * ���b�N�A�b�v�Q�[�g�ʉ߃v���O����
 */


#include "MainRunningOutCourse.h"
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


/*
 *	�e���Ԓ�`
 */

//�L�����u���[�V�����̏��
typedef enum{
	RN_SETTINGMODE_START,		//�������
	RN_RUN,						//��{���s�i���C���g���[�X�j
	RN_SLOPE,					//�⓹
	RN_RUN_SECOND,				//�⓹��̊�{���s
	RN_STEP,				//���b�N�A�b�v�Q�[�g
	RN_RUN_THIRD,				//���b�N�A�b�v�Q�[�g��̊�{���s
	RN_SEESAW,				//�h���t�g�^�[��
	RN_KEEPRUN,
	TYREAL,
} RN_SETTINGMODE;

//�������
RN_SETTINGMODE setting_mode = RN_SETTINGMODE_START;

/*
 *	�e��v���C�x�[�g�֐���`
 */

//�e��v���C�x�[�g�֐�
void RN_setting();
int RA_wheels(int turn);

//�J�E���^�̐錾
DeclareCounter(SysTimerCnt);


//�t���f�B�X�v���C�ɕ\������V�X�e�����ݒ�
const char target_subsystem_name[] = "OutCourse";



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



//���s��Ԑݒ�֐�
void RN_setting()
{
	static int timecounter = 0;
	float weight = 0.8;
	if(ecrobot_get_touch_sensor(NXT_PORT_S4) == TRUE)
			{
				setting_mode = TYREAL;
				crt_sect=START;
				ecrobot_sound_tone(880, 512, 10);
			
			}

	switch (setting_mode){
		case (TYREAL) :
			//do_tyreal(&Kp,&Ki,&Kd);
			
		TailAngleChange(ANGLEOFUP);
		PWMGeneratorModeChange(RN_MODE_STOP);

			if(ecrobot_get_touch_sensor(NXT_PORT_S4) == TRUE)
			{
				ecrobot_sound_tone(880, 512, 10);
				systick_wait_ms(500);
				setting_mode = RN_SETTINGMODE_START;
				
			}
			
			break;
			//�L�����u���[�V�������
		case (RN_SETTINGMODE_START):
			if(RN_calibrate() == 1)
			{
				setting_mode = RN_RUN;
				
				PWMGeneratorModeChange(RN_MODE_TAIL);
				
				TailAngleChange(ANGLEOFDOWN);
				
				resetSelfLocation();
				bufClear();			//�L�����u���[�V������Ɏ��Ȉʒu����֘A�̃o�b�t�@���N���A
				systick_wait_ms(500);
				
			}
			break;
	
			//�ʏ푖�s���
		case (RN_RUN):
			/*
			setCmdForward(RA_speed(getTargSpeed()));
			setCmdTurn(weight * RA_linetrace_PID(getCmdForward())+(1 - weight) * RA_curvatureCtrl_PID(getTargetR()));
			setSection_out();

			if(getInitGyroOffset() - 30 > (U32)ecrobot_get_gyro_sensor(NXT_PORT_S1) && timecounter > 500)
			{
				ecrobot_sound_tone(880, 512, 30);
				setting_mode = RN_SLOPE;
				timecounter = 0;
			}
			*/
			tailToBalance();
			break;
			
		case (RN_SLOPE):
			setSection_out();
			if(runningSlope() == 1)
				setting_mode = RN_RUN_SECOND;
			break;
			
			
		case (RN_RUN_SECOND):
			setSection_out();
			setCmdForward(RA_speed(80));
			setCmdTurn(RA_linetrace_PID(getCmdForward()));
			break;

		case (RN_STEP):
			/*
			if(runningStep() == 1)
				setting_mode = RN_RUN_THIRD;
			*/
			runningStep();
			break;
			
		case (RN_RUN_THIRD):
			setCmdForward(RA_speed(60));
			setCmdTurn(RA_linetrace_PID(getCmdForward()));
			break;

		case (RN_SEESAW):
			if(runningSeeSaw() == 1)
				setting_mode = RN_KEEPRUN;
			break;
			
		case (RN_KEEPRUN):
			setCmdForward(RA_speed(0));
			setCmdTurn(RA_linetrace_PID(0));

			/*
			setCmdForward(RA_speed(60));
			setCmdTurn(RA_linetrace_PID(getCmdForward()));
			*/
			break;
			
		default:
			break;
	}
	timecounter++;
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
	self_location();			//���Ȉʒu����
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
	logSend(R,cmd_turn,getcount(),getDistance(),ecrobot_get_battery_voltage(),ecrobot_get_gyro_sensor(NXT_PORT_S1));			//Bluetooth��p���ăf�[�^���M

	TerminateTask();
}



/******************************** END OF FILE ********************************/

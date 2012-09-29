/*
 * Slope.c
 * �⓹�v���O����
 */

#include "kernel.h"
#include "kernel_id.h"
#include "ecrobot_interface.h"
#include "Slope.h"
#include "Common/Factory.h"

#include "./DirectorPart/RunnerRobot.h"
#include "DetectionPart/Notice.h"

/*
 *	�e���`
*/

#define PI 3.141592



//�ԗւ̉~��[cm]
#define CIRCUMFERENCE 25.8			//�ԗւ̉~��



/*
 *	�O���[�o���ϐ�
 */
//�J�E���^�̐錾
DeclareCounter(SysTimerCnt);


//�^�X�N�̐錾

DeclareTask(ActionTask);
DeclareTask(ActionTask2);
DeclareTask(DisplayTask);
DeclareTask(LogTask);


//�t���f�B�X�v���C�ɕ\������V�X�e�����ݒ�
const char target_subsystem_name[] = "Slope";



/*
 *	�֐�����
 */


//���������֐��i�v���O�����̍ŏ��ɌĂяo���j
void ecrobot_device_initialize(void)
{										//�S�I�u�W�F�N�g������
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
	makeSectors();
	factory();
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



/*
 *	�e��^�X�N
 */

//���s�̊Ǘ��^�X�N(4ms)
TASK(ActionTask)
{	
	RunneRobot_run(&mRunnerRobot);
	/*
	RN_modesetting();							//���s�̏�Ԑݒ�
	TailControl_PIDTailControl(&mTailControl);	//���_:�K�����ڕW�p�x�Œ�~�����ɉ�葱����
//	taildown();									//�K������
	SelfLocation_SelfLocate(&mSelfLocation);	//���Ȉʒu����
	*/
	TerminateTask();
}

TASK(ActionTask2)
{	LocationInfo_selfLocate(&mLocationInfo); //���Ȉʒu����
	Notice_judgeNotice(&mNotice);
	/*
	RN_setting();		//���s��Ԑݒ�
	*/
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

	//logSend(SelfLocation_GetDistance(&mSelfLocation),cmd_turn,SelfLocation_GetDistance(&mSelfLocation),ecrobot_get_gyro_sensor(NXT_PORT_S1),		//Bluetooth��p���ăf�[�^���M
	//		BLACK_VALUE,WHITE_VALUE);

	

	TerminateTask();
}



/******************************** END OF FILE ********************************/

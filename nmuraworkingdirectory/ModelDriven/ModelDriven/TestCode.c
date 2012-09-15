#include "TestCode.h"
#include "Common\Factory.h"

//�J�E���^�̐錾
DeclareCounter(SysTimerCnt);

//���������֐��i�v���O�����̍ŏ��ɌĂяo���j
void ecrobot_device_initialize(void)
{
	factory();
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

//���s�̊Ǘ��^�X�N(4ms)
TASK(ActionTask)
{
	static int wait = 0;
	wait++;

	TailAngleCtrl_setTargTailAngle(&mTailAngleCtrl,95);

	ForwardValRevise_setTargForwardVal(&mForwardValRevise, 20);

	LightValCtrl_setTargLightVal(&mLightValCtrl,580);

	TailAngleCtrl_doTailCtrl(&mTailAngleCtrl);

	nxt_motor_set_speed(NXT_PORT_B,ForwardValRevise_reviseForwardVal(&mForwardValRevise) - LightValCtrl_doLightValCtrl(&mLightValCtrl) / 2,1);
	nxt_motor_set_speed(NXT_PORT_C,ForwardValRevise_reviseForwardVal(&mForwardValRevise) + LightValCtrl_doLightValCtrl(&mLightValCtrl) / 2,1);

	TerminateTask();
}

//���s��ԊǗ��^�X�N(10ms)
TASK(ActionTask2)
{
	logSend(ForwardValRevise_reviseForwardVal(&mForwardValRevise),LightValCtrl_doLightValCtrl(&mLightValCtrl),LightVal_getLightVal(&mLightVal),0,0,0);
	TerminateTask();
}

#include "TailAngleCtrl.h"
#include "../Common/Factory.h"
#include "kernel.h"
#include "kernel_id.h"

#include "../LogSend.h"
/*
 *	TailAngleCtrl.c
 *	�K���p�x�����N���X
 */

//�J�E���^�̐錾
DeclareCounter(SysTimerCnt);

//�t���f�B�X�v���C�ɕ\������V�X�e�����ݒ�
const char target_subsystem_name[] = "hoge";



//��������
void ecrobot_device_initialize(void)
{
	factory();
	ecrobot_init_bt_slave("LEJOS-OSEK");
	ecrobot_set_motor_speed(NXT_PORT_A,0);
	ecrobot_set_motor_rev(NXT_PORT_A,0);
}


//��n������
void ecrobot_device_terminate(void)
{
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

//���������\�b�h
void TailAngleCtrl_init(TailAngleCtrl *this_TailAngleCtrl)
{
}

//�ڕW�p�x�ݒ胁�\�b�h
void TailAngleCtrl_setTargTailAngle(TailAngleCtrl *this_TailAngleCtrl, int angle)
{
	TailAngle_setTargTailAngle(&mTailAngle,angle);
}

//�p�x������s���\�b�h
void TailAngleCtrl_doTailCtrl(TailAngleCtrl *this_TailAngleCtrl)
{
	TailMotor_setTailSpeed(&mTailMotor,
		PIDTailAngleCtrl_calcTailAngleCtrlVal(&mPIDTailAngleCtrl,TailAngle_getTargTailAngle(&mTailAngle),TailAngle_getTailAngle(&mTailAngle)));
}

TASK(ActionTask)
{
	static int wait = 0;
	if(wait==0)
		TailAngleCtrl_setTargTailAngle(&mTailAngleCtrl,120);
	if(wait == 1000)
		TailAngleCtrl_setTargTailAngle(&mTailAngleCtrl,0);
	TailAngleCtrl_doTailCtrl(&mTailAngleCtrl);	//���s�̏��

	logSend(0,0,TailAngle_getTargTailAngle(&mTailAngle),TailAngle_getTailAngle(&mTailAngle),PIDTailAngleCtrl_calcTailAngleCtrlVal(&mPIDTailAngleCtrl,TailAngle_getTargTailAngle(&mTailAngle),TailAngle_getTailAngle(&mTailAngle)),0);

	wait++;

	TerminateTask();
}


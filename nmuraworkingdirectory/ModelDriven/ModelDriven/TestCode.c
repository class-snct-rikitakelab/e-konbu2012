#include "TestCode.h"
#include "Common\Factory.h"

//カウンタの宣言
DeclareCounter(SysTimerCnt);

//初期処理関数（プログラムの最初に呼び出し）
void ecrobot_device_initialize(void)
{
	factory();
	ecrobot_set_light_sensor_active(NXT_PORT_S3);	//光センサ起動
	ecrobot_init_bt_slave("LEJOS-OSEK");			//Bluetooth起動
	ecrobot_init_sonar_sensor(NXT_PORT_S2);			//超音波センサ起動

	//モータリセット
	ecrobot_set_motor_rev(NXT_PORT_A,0);
	ecrobot_set_motor_rev(NXT_PORT_B,0);
	ecrobot_set_motor_rev(NXT_PORT_C,0);
	ecrobot_set_motor_speed(NXT_PORT_A,0);
	ecrobot_set_motor_speed(NXT_PORT_B,0);
	ecrobot_set_motor_speed(NXT_PORT_C,0);
}

//後始末処理関数（プログラム終了時呼び出し）
void ecrobot_device_terminate(void)
{
	ecrobot_set_light_sensor_inactive(NXT_PORT_S3);	//光センサ終了
	ecrobot_term_sonar_sensor(NXT_PORT_S2);			//超音波センサ終了
	ecrobot_term_bt_connection();					//Bluetooth終了

	//モータリセット
	ecrobot_set_motor_rev(NXT_PORT_A,0);
	ecrobot_set_motor_rev(NXT_PORT_B,0);
	ecrobot_set_motor_rev(NXT_PORT_C,0);
	ecrobot_set_motor_speed(NXT_PORT_A, 0);			
	ecrobot_set_motor_speed(NXT_PORT_B, 0);
	ecrobot_set_motor_speed(NXT_PORT_C, 0);
}

//OSEKフック関数
void StartupHook(void){}
void ShutdownHook(StatusType ercd){}
void PreTaskHook(void){}
void PostTaskHook(void){}
void ErrorHook(StatusType ercd){}

//フック関数
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

//走行体管理タスク(4ms)
TASK(ActionTask)
{/*
	static int wait = 0;
	wait++;

	if(wait < 2000)
		TailAngleCtrl_setTargTailAngle(&mTailAngleCtrl,95);

	else
		TailAngleCtrl_setTargTailAngle(&mTailAngleCtrl,0);

	TailAngleCtrl_doTailCtrl(&mTailAngleCtrl);
	*/
	TerminateTask();
}

//走行状態管理タスク(10ms)
TASK(ActionTask2)
{/*
	logSend(ForwardValRevise_reviseForwardVal(&mForwardValRevise),LightValCtrl_doLightValCtrl(&mLightValCtrl),
		LightVal_getLightVal(&mLightVal),PIDTailAngleCtrl_calcTailAngleCtrlVal(&mPIDTailAngleCtrl,TailAngle_getTargTailAngle(&mTailAngle),TailAngle_getTailAngle(&mTailAngle)),0,0);
		*/
		TerminateTask();
}

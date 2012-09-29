#include "TestCode.h"
#include "Common/Factory.h"

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
{
	RobotDrivenDirect_directDrive(&mRobotDrivenDirect);
	TerminateTask();
}

//走行状態管理タスク(10ms)
TASK(ActionTask2)
{
	Sector mSector;
	TargetDrivenParm parm;
	parm.blackVal = 560;
	parm.curvature = 0.0;
	parm.gyroOffset = 0;
	parm.runMode = BALANCING;
	parm.tailAngle = 95;
	parm.targCtrlMethod = LIGHT_PID/*CURV_PID*/;
	parm.targForwardVal = 10;
	parm.targLightVal = 600;
	parm.volt = 7800;
	parm.whiteVal = 620;

	RobotDrivenDirect_seDriveParm(&mRobotDrivenDirect,parm);
	LocationInfo_selfLocate(&mLocationInfo);
	logSend(0,0,Curvature_getCurvature(&mCurvature),Curvature_getTargCurvature(&mCurvature),0,0);
	TerminateTask();
}

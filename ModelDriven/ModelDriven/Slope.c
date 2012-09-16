/*
 * Slope.c
 * 坂道プログラム
 */

#include "kernel.h"
#include "kernel_id.h"
#include "ecrobot_interface.h"
#include "Slope.h"
#include "Common/Factory.h"

#include "./DirectorPart/RunnerRobot.h"
#include "DetectionPart/Notice.h"

/*
 *	各種定義
*/

#define PI 3.141592



//車輪の円周[cm]
#define CIRCUMFERENCE 25.8			//車輪の円周



/*
 *	グローバル変数
 */
//カウンタの宣言
DeclareCounter(SysTimerCnt);


//タスクの宣言

DeclareTask(ActionTask);
DeclareTask(ActionTask2);
DeclareTask(DisplayTask);
DeclareTask(LogTask);


//液晶ディスプレイに表示するシステム名設定
const char target_subsystem_name[] = "Slope";



/*
 *	関数実装
 */


//初期処理関数（プログラムの最初に呼び出し）
void ecrobot_device_initialize(void)
{										//全オブジェクト初期化
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
	makeSectors();
	factory();
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



/*
 *	各種タスク
 */

//走行体管理タスク(4ms)
TASK(ActionTask)
{	
	RunneRobot_run(&mRunnerRobot);
	/*
	RN_modesetting();							//走行体状態設定
	TailControl_PIDTailControl(&mTailControl);	//問題点:尻尾が目標角度で停止せずに回り続ける
//	taildown();									//尻尾制御
	SelfLocation_SelfLocate(&mSelfLocation);	//自己位置同定
	*/
	TerminateTask();
}

TASK(ActionTask2)
{	LocationInfo_selfLocate(&mLocationInfo); //自己位置推定
	Notice_judgeNotice(&mNotice);
	/*
	RN_setting();		//走行状態設定
	*/
	TerminateTask();
}

//状態表示管理タスク(20ms)
TASK(DisplayTask)
{	
	ecrobot_status_monitor(target_subsystem_name);	//モニタに状態表示
	TerminateTask();
}

//ログ送信、超音波センサ管理タスク(50ms) (共に50msでなければ動作しない）
TASK(LogTask)
{

	//logSend(SelfLocation_GetDistance(&mSelfLocation),cmd_turn,SelfLocation_GetDistance(&mSelfLocation),ecrobot_get_gyro_sensor(NXT_PORT_S1),		//Bluetoothを用いてデータ送信
	//		BLACK_VALUE,WHITE_VALUE);

	

	TerminateTask();
}



/******************************** END OF FILE ********************************/

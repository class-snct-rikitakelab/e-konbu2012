/*
 * lookupgate.c
 * ルックアップゲート通過プログラム
 */


#include "MainRunningInCourse.h"
#include "math.h"


//ジャイロ振幅値
#define PM_GYRO 65

//車輪の円周[cm]
#define CIRCUMFERENCE 25.8			//車輪の円周


/* 車輪半径、走行体幅*/
#define WHEEL_R		41	//[mm]
#define MACHINE_W	162	//[mm]

/*
 *	グローバル変数
 */


/*
 *	各種状態定義
 */

//キャリブレーションの状態
typedef enum{
	RN_SETTINGMODE_START,		//初期状態
	RN_RUN,						//基本走行（ライントレース）
	RN_SLOPE,					//坂道
	RN_RUN_SECOND,				//坂道後の基本走行
	RN_LOOKUPGATE,				//ルックアップゲート
	RN_RUN_THIRD,				//ルックアップゲート後の基本走行
	RN_DRIFTTURN,				//ドリフトターン
} RN_SETTINGMODE;

//初期状態
RN_SETTINGMODE setting_mode = RN_SETTINGMODE_START;

/*
 *	各種プライベート関数定義
 */

//各種プライベート関数
void RN_setting();
int RA_wheels(int turn);

//カウンタの宣言
DeclareCounter(SysTimerCnt);


//液晶ディスプレイに表示するシステム名設定
const char target_subsystem_name[] = "InCourse";



/*
 *	関数実装
 */


//初期処理関数（プログラムの最初に呼び出し）
void ecrobot_device_initialize(void)
{
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

//2車輪の回転量差、P制御関数（目標値に近づける）
int RA_wheels(int turn){
	float w_kp = 1.4;	//回転量P制御パラメータ

	signed long def = ecrobot_get_motor_rev(NXT_PORT_B) - ecrobot_get_motor_rev(NXT_PORT_C);	//回転量差
	turn = w_kp * def;	//回転量算出

	return turn;
}



//走行状態設定関数
void RN_setting()
{
	static int timecounter = 0;
	float weight = 0.6;
	static int markerflag = 0;


	switch (setting_mode){

			//キャリブレーション状態
		case (RN_SETTINGMODE_START):
			if(RN_calibrate() == 1)
			{
				setting_mode = RN_RUN;
				PWMGeneratorModeChange(RN_MODE_TAIL);
				TailAngleChange(ANGLEOFDOWN);
			}
			break;
	
			//通常走行状態
		case (RN_RUN):
			setCmdForward(RA_speed(80));
			setCmdTurn(weight * RA_linetrace_PID(getCmdForward())+(1 - weight) * RA_curvatureCtrl_PID(getTargetR()));
			setSection_in();
			
			if(getInitGyroOffset() - 50 > (U32)ecrobot_get_gyro_sensor(NXT_PORT_S1) && timecounter > 1000)
			{
				ecrobot_sound_tone(880, 512, 30);
				setting_mode = RN_SLOPE;
				timecounter = 0;
			}
			
			break;
			
		case (RN_SLOPE):
			setSection_in();
			if(runningSlope() == 1)
				setting_mode = RN_RUN_SECOND;
			break;
			
		case (RN_RUN_SECOND):
			setSection_in();
			setCmdForward(RA_speed(80));
			setCmdTurn(weight * RA_linetrace_PID(getCmdForward())+(1 - weight) * RA_curvatureCtrl_PID(getTargetR()));
			if(markerDetector() == 1)
				markerflag+=1;
			if(markerflag == 2)
				setting_mode = RN_LOOKUPGATE;
			break;

		case (RN_LOOKUPGATE):
			if(runningLookUpGate() == 1)
				setting_mode = RN_RUN_THIRD;
			break;
			
		case (RN_RUN_THIRD):
			setCmdForward(RA_speed(60));
			setCmdTurn(RA_linetrace_PID(getCmdForward()));
			break;
			/*
		case (RN_DRIFTTURN):
			break;
			*/
		default:
			break;
	}
	timecounter++;
}

/*
 *	各種タスク
 */

//走行体管理タスク(4ms)
TASK(ActionTask)
{
	//RN_modesetting();	//走行体状態設定
	calcPWMValues();
	TailControl();			//尻尾制御
	self_location();			//自己位置同定
	TerminateTask();
}

//走行状態管理タスク(5ms)
TASK(ActionTask2)
{
	RN_setting();		//走行状態設定
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
	logSend(markerDetector(),cmd_turn,dist,getDistance(),getInitGyroOffset(),ecrobot_get_gyro_sensor(NXT_PORT_S1));			//Bluetoothを用いてデータ送信

	getSonarValue();
	//if(markerDetector() == 1)
		//ecrobot_sound_tone(920, 512, 30);
	TerminateTask();
}



/******************************** END OF FILE ********************************/

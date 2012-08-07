/*
 * sample10.c
 *
 * バランスして立ち、ライントレースする
 * (c) COPYRIGHT 2010 Afrel., Inc.
 * All rights reserved
 */

#include "sample10.h"

/*
 * 閾値
*/
static unsigned int LIGHT_THRESHOLD=580;
static unsigned int BLACK_VALUE=600;
static unsigned int WHITE_VALUE=500;
static unsigned int GRAY_VALUE=550;

static float hensa;
	static float i_hensa = 0;
	static float d_hensa = 0;
	static float bf_hensa = 0;
	static float Kp = 1.75;
	static float Ki = 2.6;
	static float Kd = 0.01;

/*
 * システム全体の状態
 */
typedef enum{
	RN_MODE_INIT, 		/* system initialize mode */
	RN_MODE_CONTROL,		/* control mode */
	RN_RUN
} RN_MODE;


/*
 * キャリブレーションの状態
 */
typedef enum{
	RN_SETTINGMODE_GYRO_START,
	RN_SETTINGMODE_END
} RN_SETTINGMODE;


/*
 *初期状態
 */

RN_MODE runner_mode = RN_MODE_INIT;					/* NXTway-GS mode */
RN_SETTINGMODE setting_mode = RN_SETTINGMODE_GYRO_START;/* キャリブレーションの状態 */


/*
 * ジャイロセンサオフセット計算用変数
 */
static U32	gyro_offset = 0;    /* gyro sensor offset value */
static U32	avg_cnt = 0;		/* average count to calc gyro offset */
static U32	cal_start_time;		/* calibration start time */
static U32	battery_value = 0;


/* バランスコントロールへ渡すコマンド用変数 */
S8  cmd_forward, cmd_turn;
/* バランスコントロールから返されるモータ制御用変数 */
S8	pwm_l, pwm_r;
	
	
/*
 * キャリブレーション用のプライベート関数
 */
void RN_calibrate();
void RN_setting();
int online();

/*
 * ロボット制御用のプライベート関数
 */
int RN_move();

/*
 *カウンタの宣言
 */
DeclareCounter(SysTimerCnt);


/*
 *タスクの宣言
 */
DeclareTask(ActionTask);
DeclareTask(ActionTask2);
DeclareTask(DisplayTask);
DeclareTask(LogTask);


/*
 *液晶ディスプレイに表示するシステム名設定
 */
const char target_subsystem_name[] = "Logtrace";


/*
 *初期処理
 */
void ecrobot_device_initialize(void)
{
	ecrobot_set_light_sensor_active(NXT_PORT_S3);
	ecrobot_init_bt_slave("LEJOS-OSEK");
}


/*
 *後始末処理
 */
void ecrobot_device_terminate(void)
{
	ecrobot_set_motor_speed(NXT_PORT_B, 0);
	ecrobot_set_motor_speed(NXT_PORT_C, 0);
	ecrobot_set_light_sensor_inactive(NXT_PORT_S3);
	ecrobot_term_sonar_sensor(NXT_PORT_S2);
	ecrobot_term_bt_connection();
}


/*--------------------------------------------------------------------------*/
/* OSEK hooks                                                               */
/*--------------------------------------------------------------------------*/
void StartupHook(void){}
void ShutdownHook(StatusType ercd){}
void PreTaskHook(void){}
void PostTaskHook(void){}
void ErrorHook(StatusType ercd){}


/*--------------------------------------------------------------------------*/
/* Function to be invoked from a category 2 interrupt                       */
/*--------------------------------------------------------------------------*/
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
 *タスク1用の関数
 */
TASK(ActionTask)
{

	switch (runner_mode){
		case (RN_MODE_INIT):
			cmd_forward = 0;
			cmd_turn = 0;
			battery_value = ecrobot_get_battery_voltage();
			break;

		case (RN_MODE_CONTROL):
			balance_control(
				(F32)cmd_forward,
				(F32)cmd_turn,
				(F32)ecrobot_get_gyro_sensor(NXT_PORT_S4),
				(F32)gyro_offset,
				(F32)nxt_motor_get_count(NXT_PORT_C),
				(F32)nxt_motor_get_count(NXT_PORT_B),
				(F32)ecrobot_get_battery_voltage(),
				&pwm_l,
				&pwm_r);
			nxt_motor_set_speed(NXT_PORT_C, pwm_l, 1);
			nxt_motor_set_speed(NXT_PORT_B, pwm_r, 1);
			break;

		default:
			nxt_motor_set_speed(NXT_PORT_C, 0, 1);
			nxt_motor_set_speed(NXT_PORT_B, 0, 1);
			break;
	}
	TerminateTask();
}


/*
 *表示用の関数
 */



/*
 *タスク2用の関数
 */
TASK(ActionTask2)
{
	RN_setting();

	/* 自タスクの終了 */
	/* 具体的には，自タスクを実行状態から休止状態に移行させ，*/
	/* タスクの終了時に行うべき処理を行う */
	TerminateTask();
}



int online(void) {
	int light_value;
	light_value = ecrobot_get_light_sensor(NXT_PORT_S3);
	if (GRAY_VALUE > light_value) {
		if ((GRAY_VALUE - 20) > light_value) {
			return FALSE;
		} else {
			return TRUE;
		}
	}
	return TRUE;
}

TASK(LogTask)
{
	static S8 i,j;
	int sonar;

	sonar = ecrobot_get_sonar_sensor(NXT_PORT_S2);

	ecrobot_bt_data_logger(hensa, cmd_turn);

	/* display Sensors/Motors/NXT internal status */ 
	ecrobot_status_monitor("Data Logging");

	TerminateTask();
}

/*
 *プライベート関数の実装
 */
void RN_setting()
{
	switch (setting_mode){
		case (RN_SETTINGMODE_GYRO_START):
			RN_calibrate();
			break;

		case (RN_SETTINGMODE_END):
			runner_mode = RN_MODE_CONTROL;
			setting_mode=RN_RUN;

		case (RN_RUN):
			RA_linetrace_P(30);
			break;
		default:
			break;
	}
}

/******************************** END OF FILE ********************************/

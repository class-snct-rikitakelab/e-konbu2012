/*
 * RadioControl.c
 * ETバトルプログラム
 */

#include "RadioControl.h"

/*
 *	各種定義
*/

/* バランスコントロールへ渡すコマンド用変数 */
S8  cmd_forward, cmd_turn;
/* バランスコントロールから返されるモータ制御用変数 */
S8	pwm_l, pwm_r;

static U32	gyro_offset = 0;    /* gyro sensor offset value */

/*
 *	各種状態定義
 */


//システム全体の状態
typedef enum{
	RN_MODE_INIT, 					//初期状態
	RN_MODE_BALANCE,				//倒立制御ON
	RN_MODE_BALANCEOFF				//倒立制御OF
} RN_MODE;


//キャリブレーションの状態
typedef enum{
	RN_START,					//開始待ちモード
	RN_RUN,						//対戦モード
} RN_SETTINGMODE;

//初期状態
RN_MODE runner_mode = RN_MODE_INIT;
RN_SETTINGMODE setting_mode = RN_START;

/*
 *	各種プライベート関数定義
 */

//各種プライベート関数
void RN_calibrate();
void RN_setting();
void RN_modesetting();

#define BT_RCV_BUF_SIZE (32) /* it must be 32bytes with NXT GamePad */
static U8 bt_receive_buf[BT_RCV_BUF_SIZE]; /* Bluetooth receive buffer(32bytes) */

//カウンタの宣言
DeclareCounter(SysTimerCnt);

//液晶ディスプレイに表示するシステム名設定
const char target_subsystem_name[] = "RadioControl";

/*
 *	関数実装
 */


//初期処理関数（プログラムの最初に呼び出し）
void ecrobot_device_initialize(void)
{
	ecrobot_set_light_sensor_active(NXT_PORT_S3);	//光センサ起動
	ecrobot_init_bt_slave("LEJOS-OSEK");			//Bluetooth起動

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
	ercd = SignalCounter( SysTimerCnt );
	if( ercd != E_OK ){
		ShutdownOS( ercd );
	}
}


//走行状態設定関数
void RN_setting()
{
	ecrobot_bt_data_logger(cmd_turn, cmd_forward);

	switch (setting_mode){

			//キャリブレーション状態
		case (RN_START):
			RN_calibrate();
			break;
		
			//通常走行状態
		case (RN_RUN):
			(void)ecrobot_read_bt_packet(bt_receive_buf, BT_RCV_BUF_SIZE);
			
			cmd_forward = -((S8)bt_receive_buf[0]);
			cmd_turn = ((S8)bt_receive_buf[1]);
			break;

		default:
			break;
	}
}

//キャリブレーション関数
void RN_calibrate()
{
	/*バランサーON用*/
	
	while(1){
		if(ecrobot_get_touch_sensor(NXT_PORT_S4) == TRUE)
		{
			ecrobot_sound_tone(932, 512, 10);
			gyro_offset += (U32)ecrobot_get_gyro_sensor(NXT_PORT_S1);
			systick_wait_ms(500);
			break;
		}
	}
	
	while(1){
		if(ecrobot_get_touch_sensor(NXT_PORT_S4) == TRUE)
		{
			ecrobot_sound_tone(982,512,10);
			setting_mode = RN_RUN;
			runner_mode = RN_MODE_BALANCE;
			systick_wait_ms(500);
			break;
		}
	}

}

//走行体状態設定関数
void RN_modesetting()
{
	switch (runner_mode){
			//走行体初期状態
		case (RN_MODE_INIT):
			cmd_forward = 0;
			cmd_turn = 0;
			break;

			//バランサー
		case (RN_MODE_BALANCE):
			balance_control(
				(F32)cmd_forward,
				(F32)cmd_turn,
				(F32)ecrobot_get_gyro_sensor(NXT_PORT_S1),
		 		(F32)gyro_offset,
				(F32)nxt_motor_get_count(NXT_PORT_C),
		 		(F32)nxt_motor_get_count(NXT_PORT_B),
				(F32)ecrobot_get_battery_voltage(),
				&pwm_l,
				&pwm_r);
			nxt_motor_set_speed(NXT_PORT_C, pwm_l, 1);
			nxt_motor_set_speed(NXT_PORT_B, pwm_r, 1);
			break;

			//バランサー無し
		case (RN_MODE_BALANCEOFF):
			break;

		default:
			nxt_motor_set_speed(NXT_PORT_C, 0, 1);
			nxt_motor_set_speed(NXT_PORT_B, 0, 1);
			break;
	}
}


/*
 *	各種タスク
 */

//走行体管理タスク(4ms)
TASK(ActionTask)
{
	RN_modesetting();	//走行体状態設定
	RN_setting();		//走行状態設定
	TerminateTask();
}

/******************************** END OF FILE ********************************/

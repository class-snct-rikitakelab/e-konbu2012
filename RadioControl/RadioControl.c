/*
 * RadioControl.c
 * ETバトルプログラム
 */

#include "RadioControl.h"

/*
 *	各種定義
*/

//尻尾設定角度
#define ANGLEOFDOWN 105 				//降下目標角度
#define ANGLEOFUP 0						//上昇目標角度

/*
 *	グローバル変数
 */

//尻尾PI制御用係数
static float t_Kp = 3.85;			//P制御用

//尻尾制御用変数
static int t_angle = 0;				//現在の角度
static int t_count = 0;				//制御速度調節用カウンタ
static int t_value = 0;				//角度目標値
static int t_count_limit = 0;		//カウンタ最大値
static int t_up = 0;				//増減値

//全体用カウンタ（時間稼ぎ用）
static int wait_count = 0;

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
	RN_PUSHBUTTON				//ボタン押下モード
} RN_SETTINGMODE;

//尻尾の状態
typedef enum{
	RN_TAILDOWN,				//尻尾降下
	RN_TAILUP,					//尻尾上昇
} RN_TAILMODE;

//初期状態
RN_MODE runner_mode = RN_MODE_INIT;
RN_SETTINGMODE setting_mode = RN_START;
RN_TAILMODE tail_mode = RN_TAILUP;

/*
 *	各種プライベート関数定義
 */


//各種プライベート関数
void RN_calibrate();
void RN_setting();
void RN_modesetting();
void runner_mode_change(int flag);
void tailpower(float value);
void tail_mode_change(int mode,int value,int limit,int t_up);
void taildown();

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
	tail_mode = RN_TAILUP;							//尻尾を上げる

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

//尻尾角度調節関数
void taildown(){

	//X-ecrobot_get_motor_rev(NXT_PORT_S4) のX = 目標値
	//目標値を1ずつ調節(現行目標値t_angleそのものを最終目標値t_valueへ近づけていく)
	//t_angleの初期値は現在の角度

	static float t_hensa = 0;	//尻尾角度の目標値との差
	static float t_speed = 0;	//尻尾モータに送る速度

	t_count++;					//速度制御用カウンタ

	switch(tail_mode){
		case(RN_TAILDOWN):				//尻尾を下げる
			if(t_angle <= t_value)		//現在の角度が目標値以下かどうか
			{
				t_hensa = t_angle - ecrobot_get_motor_rev(NXT_PORT_A);
				if(t_count >= t_count_limit)	//カウンタで尻尾を下げる速度を調整
				{
					t_angle+=t_up;			//角度を上げる
					t_count = 0;
				}
			}
			else
			{
				t_angle = t_value;
			}

			break;

		case(RN_TAILUP):				//尻尾を上げる
			if(t_angle >= t_value)		//現在の角度が目標値以上かどうか
			{
				t_hensa = t_angle - ecrobot_get_motor_rev(NXT_PORT_A);
				if(t_count >= t_count_limit)	//カウンタで尻尾を上げる速度を調整
				{
					t_angle-=t_up;			//角度を下げる
					t_count = 0;
				}
			}
			else
			{
				t_angle = t_value;
			}

			break;

		default:
			break;

	}

	t_speed = t_Kp*t_hensa;	//偏差を元に速度調節

	if (t_speed < -100)
		t_speed = -100;
	else if (t_speed > 100)
		t_speed = 100;

	ecrobot_set_motor_speed(NXT_PORT_A, t_speed);	//モータに速度を送る

}

//尻尾モード変更関数
void tail_mode_change(int mode,int value,int limit,int up)	//mode(0:尻尾を下ろす、1:尻尾を上げる) value(目標値) limit(上げるほど遅延） up（上げるほど加速）1
{

	static int flag;					//尻尾モードフラグ

	switch(mode){
		case 0:
			tail_mode = RN_TAILDOWN;	//尻尾を下ろす
			flag = 0;
			break;
		case 1:
			tail_mode = RN_TAILUP;		//尻尾を上げる
			flag = 1;
			break;
		default:
			break;
	}

	//目標値が変わった時のみ、角度を決める（複数回呼び出す場合も初期値をキープするため）
	if(t_value != value)
		t_angle = ecrobot_get_motor_rev(NXT_PORT_A);

	t_value = value;			//目標値設定
	t_count_limit = limit;		//カウンタ最大値設定
	t_up = up;					//角度増減値設定

}

//走行状態設定関数
void RN_setting()
{

	switch (setting_mode){

			//キャリブレーション状態
		case (RN_START):
			RN_calibrate();
			break;
		
			//通常走行状態
		case (RN_RUN):
			(void)ecrobot_read_bt_packet(bt_receive_buf, BT_RCV_BUF_SIZE);
			
			cmd_forward = -((S8)bt_receive_buf[0]/2);
			cmd_turn = ((S8)bt_receive_buf[1]/2);
			
			nxt_motor_set_speed(NXT_PORT_C, cmd_forward + cmd_turn/2, 1);
			nxt_motor_set_speed(NXT_PORT_B, cmd_forward - cmd_turn/2, 1);
		
			if(ecrobot_get_touch_sensor(NXT_PORT_S4) == 1)
			{
				ecrobot_sound_tone(980,512,30);
				nxt_motor_set_speed(NXT_PORT_C,100,1);
				nxt_motor_set_speed(NXT_PORT_B,-100,1);
				wait_count = 0;
				setting_mode = RN_PUSHBUTTON;
			}
			
			break;

			//ルックアップゲート走行準備状態
		case (RN_PUSHBUTTON):
			wait_count++;

			if(wait_count >= 150)
			{
				tail_mode_change(1,ANGLEOFUP,1,1);
				nxt_motor_set_speed(NXT_PORT_C,0,1);
				nxt_motor_set_speed(NXT_PORT_B,0,1);
			}

			if(wait_count == 1000)
				setting_mode = RN_START;

			break;

		default:
			break;
	}
}

//キャリブレーション関数
void RN_calibrate()
{
	/*バランサーON用
	while(1){
		if(ecrobot_get_touch_sensor(NXT_PORT_S4) == TRUE)
		{
			ecrobot_sound_tone(932, 512, 10);
			gyro_offset += (U32)ecrobot_get_gyro_sensor(NXT_PORT_S1);
			systick_wait_ms(500);
			break;
		}
	}
	*/
	while(1){
		if(ecrobot_get_touch_sensor(NXT_PORT_S4) == TRUE)
		{
			ecrobot_sound_tone(982,512,10);
			tail_mode_change(0,ANGLEOFDOWN,1,2);
			setting_mode = RN_RUN;
			runner_mode = RN_MODE_BALANCEOFF;
//			runner_mode = RN_MODE_BALANCE;
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
	taildown();			//尻尾制御
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

/******************************** END OF FILE ********************************/

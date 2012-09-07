/*
 * MainRunningInCourse.c
 * インコース走行プログラム
 */


#include "MainRunningInCourse.h"

/*
 *	各種定義
*/

//速度カウンタの上限値
#define SPEED_COUNT 10


/*
 *	グローバル変数
 */



//速度調節カウンタ（カウンタが上限に達する毎に速度が1上昇
static int speed_counter = 0;

//尻尾制御用変数
static int t_angle = 0;				//現在の角度
static int t_count = 0;				//制御速度調節用カウンタ
static int t_value = 0;				//角度目標値
static int t_count_limit = 0;		//カウンタ最大値
static int t_up = 0;				//増減値

//全体用カウンタ（時間稼ぎ用）
static int time_counter = 0;

//超音波センサ目標値
static int target_sonar = 20;

//超音波センサ用フラグ
static int sonarflag;

static int sonarvalue;

//ジャイロセンサオフセット計算用変数
static U32	gyro_offset = 0;    /* gyro sensor offset value */

char rx_buf[BT_MAX_RX_BUF_SIZE];

/* バランスコントロールへ渡すコマンド用変数 */
S8  cmd_forward, cmd_turn;
/* バランスコントロールから返されるモータ制御用変数 */
S8	pwm_l, pwm_r;


//距離計測用変数
int revL = 0;
int revR = 0;

int distance_before_gate;	//ルックアップゲート通過前距離
int distance_after_gate;	//ルックアップゲート通過中距離


//マーカーフラグ　0: OFF, 1: ON
unsigned char m_flg = 0;
static unsigned int LV_buf = 0;		/* Light Value buffer */

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
	RN_SETTINGMODE_START,		//初期状態
	RN_SPEEDZERO,				//速度ゼロキープ
	RN_RUN,						//基本走行（ライントレース）
	RN_LOOKUP_START,
	RN_LOOKUP_STOP,					//ルックアップゲート準備
	RN_LOOKUP_DOWN,				//走行体降下
	RN_LOOKUP_MOVE,				//走行体前進
	RN_LOOKUP_UP					//走行体復帰
} RN_SETTINGMODE;



//初期状態
RN_MODE runner_mode = RN_MODE_INIT;
RN_SETTINGMODE setting_mode = RN_SETTINGMODE_START;
RN_TAILMODE tail_mode = RN_TAILUP;

/*
 *	各種プライベート関数定義
 */


//各種プライベート関数
void RN_setting();
int online();
void RA_linetrace(int forward_speed, int turn_speed);
void RA_linetrace_PID(int forward_speed);
void logSend(S8 data1, S8 data2, S16 adc1, S16 adc2, S16 adc3, S16 adc4);
void taildown();
void RA_linetrace_P(int forward_speed);
void RA_speed(int limit,int s_Kp);
int RA_wheels(int turn);
void RN_modesetting();
static int remote_start(void);
int sonarcheck(int target_sonar);
void runner_mode_change(int flag);
void tailpower(float value);
void tail_mode_change(int mode,int value,int limit,int t_up);
void RA_linetrace_PID_balanceoff(int forward_speed);
void getsonarvalue(void);

void RA_hensareset(void);


void self_location(void);
unsigned char MKTrigger(void);
signed char LVTrigger(void);
int abs(int n);

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
	tail_mode = RN_TAILUP;							//尻尾を上げる（意味あるのか？）

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


//ON-OFF制御用ライン判定関数
int online(void) {

	int light_value;
	light_value = ecrobot_get_light_sensor(NXT_PORT_S3);	//現在の輝度値
					
	if (LOOKUP_GRAY_VALUE > light_value)		//輝度値が目標値より大きいか判断
		return FALSE;					//ライン外
	else
		return TRUE;					//ライン内

}

void RA_hensareset(void)
{
	hensa = 0;
	i_hensa = 0;
	d_hensa = 0;
	bf_hensa = 0;
}

//超音波センサ状態検出関数
int sonarcheck(int target_sonar)
{
	if(sonarvalue <= target_sonar)	//超音波センサの値が目標値以下か判断しフラグ変更
	{
		return 1;
	}
	else
		return 0;
}

void getsonarvalue(void)
{
	sonarvalue = ecrobot_get_sonar_sensor(NXT_PORT_S2);
}



//リモートスタート管理関数
static int remote_start(void)
{
	int i;
	unsigned int rx_len;
	unsigned char start = 0;		//状態フラグ

	for (i=0; i<BT_MAX_RX_BUF_SIZE; i++)
	{
		rx_buf[i] = 0; //受信バッファをクリア
	}

	rx_len = ecrobot_read_bt(rx_buf, 0, BT_MAX_RX_BUF_SIZE);
	if (rx_len > 0)
	{
		//受信データあり
		if (rx_buf[0] == CMD_START)
		{
			start = 1; //走行開始フラグ
		}
		ecrobot_send_bt(rx_buf, 0, rx_len); //受信データをエコーバック
	}

	return start;
}

//走行状態設定関数
void RN_setting()
{

	switch (setting_mode){

			//キャリブレーション状態
		case (RN_SETTINGMODE_START):
			RN_calibrate();
			time_counter = 0;
			break;

		case (RN_SPEEDZERO):
			cmd_forward = 0;
			cmd_turn = RA_wheels(cmd_turn);
			time_counter++;
			if(time_counter >= 100)
			{
				setting_mode = RN_RUN;
				time_counter = 0;
			}
			break;
		
			//通常走行状態
		case (RN_RUN):
			RA_linetrace_PID_balanceoff(25);
			
			setting_mode = RN_LOOKUP_START;
			
			break;

		case (RN_LOOKUP_START):
			RA_linetrace_PID_balanceoff(25);

			time_counter++;

			if(time_counter >= 150)					//スタート時に反応するのを防ぐ（テスト用）
			{
				if(sonarcheck(19) == 1)				//超音波センサが反応したかどうか
				{
					ecrobot_sound_tone(900,512,30);
					setting_mode = RN_LOOKUP_STOP;
					time_counter = 0;
				}
			}
			break;

			//ルックアップゲート走行準備状態
		case (RN_LOOKUP_STOP):
			RA_linetrace_PID_balanceoff(5);
			//cmd_turn = RA_wheels(cmd_turn);

			time_counter++;

			if(time_counter == 200)
			{
				//tail_mode_change(0,ANGLEOFDOWN,2,1);
				
				while(time_counter <= 1200 || t_angle <= ANGLEOFDOWN)
				{
					RA_speed(-20,5);
					cmd_turn = RA_wheels(cmd_turn);
					time_counter++;
				}
				setting_mode = RN_LOOKUP_DOWN;
				time_counter = 0;
				runner_mode_change(2);
			}

			break;
		
			//ルックアップゲート走行、尻尾降下
		case (RN_LOOKUP_DOWN):
			nxt_motor_set_speed(NXT_PORT_C, 0, 1);
			nxt_motor_set_speed(NXT_PORT_B, 0, 1);

			time_counter++;

			if(time_counter >= 200)
			{
				tailpower(15.0);
				tail_mode_change(1,ANGLEOFLOOKUP,4,1);
				if(ecrobot_get_motor_rev(NXT_PORT_A) == ANGLEOFLOOKUP)
					{
						setting_mode = RN_LOOKUP_MOVE;
						time_counter = 0;
						revL = nxt_motor_get_count(NXT_PORT_C);
						revR = nxt_motor_get_count(NXT_PORT_B);
						distance_before_gate = fabs(CIRCUMFERENCE/360.0 * ((revL+revR)/2.0));
					}
			}

			break;

			//ルックアップゲート走行、尻尾降下状態で前進
		case (RN_LOOKUP_MOVE):

//			RA_linetrace(30,20);
			RA_linetrace_PID_balanceoff(25);
			revL = nxt_motor_get_count(NXT_PORT_C);
			revR = nxt_motor_get_count(NXT_PORT_B);
			distance_after_gate = fabs(CIRCUMFERENCE/360.0 * ((revL+revR)/2.0));
			
			if(distance_after_gate - distance_before_gate > 30)
			{	
				setting_mode = RN_LOOKUP_UP;
			}
			
			break;

			//ルックアップゲート走行、前進後倒立状態へ復帰
		case (RN_LOOKUP_UP):
			if(time_counter < 200)
			{
				nxt_motor_set_speed(NXT_PORT_C, 0, 1);
				nxt_motor_set_speed(NXT_PORT_B, 0, 1);
			}
			time_counter++;

			//尻尾持ち上げ補助
			if(time_counter == 200)
			{
				tail_mode_change(0,ANGLEOFDOWN,4,1);
				
				ecrobot_set_motor_speed(NXT_PORT_B, -15);	//モータに速度を送る
				ecrobot_set_motor_speed(NXT_PORT_C, -15);	//モータに速度を送る
			}

			if(t_angle == ANGLEOFDOWN)
			{
				ecrobot_set_motor_speed(NXT_PORT_B, 0);	//モータに速度を送る
				ecrobot_set_motor_speed(NXT_PORT_C, 0);	//モータに速度を送る	
			}
			/*
			if(t_angle == ANGLEOFDOWN && time_counter >= 1000)
			{
				tail_mode_change(1,ANGLEOFPUSH,0,10);
				nxt_motor_set_speed(NXT_PORT_C, 0, 1);
				nxt_motor_set_speed(NXT_PORT_B, 0, 1);
			}
			*/
			if(t_angle == ANGLEOFDOWN && time_counter >= 1050)
			{
				tailpower(1.85);			

				//tail_mode_change(1,ANGLEOFUP,0,2);

				ecrobot_set_motor_rev(NXT_PORT_B,0);
				ecrobot_set_motor_rev(NXT_PORT_C,0);
				ecrobot_set_motor_speed(NXT_PORT_B,0);
				ecrobot_set_motor_speed(NXT_PORT_C,0);
				/*
				runner_mode_change(1);
				RA_hensareset();
				balance_init();
				time_counter=0;
				cmd_forward=0;
				*/
				setting_mode = RN_LOOKUP_START;
			}
			
			break;

		default:
			break;
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
	self_location();	//自己位置同定
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
	logSend(cmd_forward,cmd_turn,BLACK_VALUE,0,		//Bluetoothを用いてデータ送信
			x_r,y_r);

	getsonarvalue();

	TerminateTask();
}



/******************************** END OF FILE ********************************/

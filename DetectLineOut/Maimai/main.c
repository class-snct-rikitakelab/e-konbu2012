/*
 * main.c
 * 
 */

#include "kernel.h"
#include "kernel_id.h"
#include "ecrobot_interface.h"
#include "balancer.h" /* 倒立振子制御用ヘッダファイル */
#include "logSend.h"
#include "TimerUnit.h"
#include "DetectOnLine.h"

//ライン外れた検出用変数


//ライントレース用目標値
static unsigned int black_value;	//黒値
static unsigned int white_value;	//白値
static unsigned int gray_value;		//灰色値（現在は黒と白の平均値）
static unsigned int line_mokuhyou_value;		//ライントレース目標値

static int counter = 0;

//尻尾設定角度
#define ANGLEOFDOWN 118				//降下目標角度
#define ANGLEOFUP 0					//上昇目標角度
#define ANGLEOFPUSH 210				//押上目標角度（未使用）

//速度調節係数
#define SPEED_COUNT 20

//バッテリ降下値
#define DOWN_BATTERY 450			//バッテリ降下値

//ジャイロ振幅値
#define PM_GYRO 65

//車輪の円周[cm]
#define CIRCUMFERENCE 25.8			//車輪の円周

#define CMD_START '1'    			//リモートスタートコマンド(変更禁止)

#define MANDATORY_START_RUN_SIGNAL '2' //ラインを外れるための走行への状態遷移信号

//PID制御用偏差値
static float hensa;					//P制御用
static float i_hensa = 0;			//I制御用
static float d_hensa = 0;			//D制御用
static float bf_hensa = 0;


//ライントレース時PID制御用係数
static float Kp = 1.85;				//P制御用
static float Ki = 2.6;				//I制御用
static float Kd = 0.003;				//D制御用
//static float Kp = 0.648;
//static float Ki = 0.35;
//static float Kd = 0.0205;

static int wait_count = 0;

static int mode_count = 0;

static double min_vol;

//ジャイロセンサオフセット計算用変数
static U32	gyro_offset = 0;    /* gyro sensor offset value */

//バッテリ電圧値状態
static U32	battery_value;

char rx_buf[BT_MAX_RX_BUF_SIZE];

/* バランスコントロールへ渡すコマンド用変数 */
S8  cmd_forward, cmd_turn;
/* バランスコントロールから返されるモータ制御用変数 */
S8	pwm_l, pwm_r;


//距離計測用変数
int revL = 0;
int revR = 0;
float distance_now = 0;
float distance_before = 0;
float distance_slow = 0;
float distance_stop = 0;


//システム全体の状態
typedef enum{
	RN_MODE_INIT, 					//初期状態
	RN_MODE_CONTROL,				//倒立制御ON
	RN_MODE_STOP,					//倒立制御OFF
} RN_MODE;


//走行状態切替用のenum
typedef enum{
	RN_SETTINGMODE_START,		//初期状態
	RN_RUN,						//基本走行（ライントレース）
	RN_STOP,						//停止
	RN_MANDATORY_LINE_OUT
} RN_SETTINGMODE;


//尻尾の状態
typedef enum{
	RN_TAILDOWN,				//尻尾降下
	RN_TAILUP,					//尻尾上昇
	RN_TAILPUSH					//尻尾押出
} RN_TAILMODE;


//初期状態
RN_MODE runner_mode = RN_MODE_INIT;
RN_SETTINGMODE setting_mode = RN_SETTINGMODE_START;
RN_TAILMODE tail_mode = RN_TAILDOWN;


//各種プライベート関数
void RN_stop();
void RN_calibrate();
void RN_run_state_change();
int online();
void RA_linetrace(int forward_speed, int turn_speed);
void RA_linetrace_PID(int forward_speed);
void logSend(S8 data1, S8 data2, S16 adc1, S16 adc2, S16 adc3, S16 adc4);
void shock();
void taildown();
void RA_linetrace_P(int forward_speed);
void RA_speed(int limit,int s_Kp);
int RA_wheels(int turn);
void RN_modesetting();
static int remote_start(void);

static int RN_mandatory_run_remotestart(void); //ライン無視走行切り替え信号用関数
void initialze_PID_param();


//ライン外れるための関数
void RN_mandatory_line_out(U8 port_B_speed,U8 port_C_speed);

//カウンタの宣言
DeclareCounter(SysTimerCnt);


//タスクの宣言
/*
DeclareTask(ActionTask);
DeclareTask(ActionTask2);
DeclareTask(DisplayTask);
DeclareTask(LogTask);
*/

//液晶ディスプレイに表示するシステム名設定
const char target_subsystem_name[] = "DetectLineOut";


//初期処理
void ecrobot_device_initialize(void)
{
	ecrobot_set_light_sensor_active(NXT_PORT_S3);
	ecrobot_init_bt_slave("LEJOS-OSEK");

	ecrobot_set_motor_rev(NXT_PORT_A,0);
	ecrobot_set_motor_rev(NXT_PORT_B,0);
	ecrobot_set_motor_rev(NXT_PORT_C,0);
	ecrobot_set_motor_speed(NXT_PORT_A,0);
	ecrobot_set_motor_speed(NXT_PORT_B,0);
	ecrobot_set_motor_speed(NXT_PORT_C,0);
}


/*
 *後始末処理
 */
void ecrobot_device_terminate(void)
{
	tail_mode = RN_TAILUP;
	ecrobot_set_motor_speed(NXT_PORT_A, 0);
	ecrobot_set_motor_speed(NXT_PORT_B, 0);
	ecrobot_set_motor_speed(NXT_PORT_C, 0);
	ecrobot_set_light_sensor_inactive(NXT_PORT_S3);
	ecrobot_term_sonar_sensor(NXT_PORT_S2);
	ecrobot_term_bt_connection();
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


//ON-OFF制御ライントレース
void RA_linetrace(int forward_speed, int turn_speed) {

	cmd_forward = forward_speed;

	int light_value = 0;
	light_value = online();
	if (TRUE != light_value) {
		cmd_turn = turn_speed;
	} else {
		cmd_turn = (-1)*turn_speed;
	}

}


//P制御のみのライントレース
void RA_linetrace_P(int forward_speed){

	cmd_forward = forward_speed;

	hensa = (float)line_mokuhyou_value - (float)ecrobot_get_light_sensor(NXT_PORT_S3);

	cmd_turn = -(1.4 * hensa);
	if (-100 > cmd_turn) {
		cmd_turn = -100;
	} else if (100 < cmd_turn) {
		cmd_turn = 100;
	}
}


//PID制御ライントレース
void RA_linetrace_PID(int forward_speed) {

	if(runner_mode == RN_MODE_CONTROL){
		shock();
	}
	RA_speed(forward_speed,2);	//速度を段階的に変化
	cmd_forward = forward_speed;

	if(forward_speed > 0)
		hensa = (float)line_mokuhyou_value - (float)ecrobot_get_light_sensor(NXT_PORT_S3);
	else
		hensa = (float)ecrobot_get_light_sensor(NXT_PORT_S3) - (float)line_mokuhyou_value;

	i_hensa = i_hensa+(hensa*0.0004/*0.0005*/);
	d_hensa = (hensa - bf_hensa)/0.0004/*0.0005*/;
	bf_hensa = hensa;

	cmd_turn = -(Kp * hensa + Ki * i_hensa + Kd * d_hensa);
	if (-100 > cmd_turn) {
		cmd_turn = -100;
	} else if (100 < cmd_turn) {
		cmd_turn = 100;
	}


	/*倒立制御OFF時*/
	//nxt_motor_set_speed(NXT_PORT_C, forward_speed + cmd_turn/2, 1);
	//nxt_motor_set_speed(NXT_PORT_B, forward_speed - cmd_turn/2, 1);

}

void RA_hensareset(void)
{
	hensa = 0;
	i_hensa = 0;
	d_hensa = 0;
	bf_hensa = 0;
}

//段階的加速	
void RA_speed(int limit,int s_Kp){

	static int forward_speed;
//	static int counter = 0;

	counter += 1;

	if(counter == SPEED_COUNT)
	{

		forward_speed = cmd_forward;

		if(limit-forward_speed >= 0){
			forward_speed += s_Kp;

			if(forward_speed > limit)
				forward_speed = limit;
		}
		else{
			forward_speed -= s_Kp;

			if(forward_speed < limit)
				forward_speed = limit;
		}

		cmd_forward = forward_speed;
		counter =0;
	}
}


//2車輪の回転量の差をP制御で調節する（目標値に近づける）
int RA_wheels(int turn){
	float w_kp = 1.4;

	signed long def = ecrobot_get_motor_rev(NXT_PORT_B) - ecrobot_get_motor_rev(NXT_PORT_C);
	turn = w_kp * def;

	return turn;
}


//衝撃検知
void shock(void){
	/*
	//最小値設定
	if(min_vol>ecrobot_get_battery_voltage())
		min_vol=ecrobot_get_battery_voltage();

	//ジャイロ及び電圧降下をフラグとする
	if((ecrobot_get_gyro_sensor(NXT_PORT_S1) <= gyro_offset-PM_GYRO ||
		ecrobot_get_gyro_sensor(NXT_PORT_S1) >= gyro_offset+PM_GYRO) &&
			min_vol <= battery_value-DOWN_BATTERY)
	{
		ecrobot_sound_tone(880,512,30);
		//バランサーがモード1かどうか
		if(runner_mode == RN_MODE_CONTROL){
			revL = nxt_motor_get_count(NXT_PORT_C);
			revR = nxt_motor_get_count(NXT_PORT_B);
			distance_before = fabs(CIRCUMFERENCE/360.0 * ((revL+revR)/2.0));
			setting_mode = RN_SLOW_RUN;
			runner_mode = RN_MODE_CONTROL_2;	//繰り返し検知を防ぐためにバランサーがモード2
			gyro_offset -= 10;					//ジャイロオフセット切り替え
			min_vol = battery_value;			//最小値リセット
		}
	}
	*/
}


//ON-OFF制御用ライン判定
int online(void) {

	int light_value;
	light_value = ecrobot_get_light_sensor(NXT_PORT_S3);

	if (line_mokuhyou_value > light_value) {
		if ((line_mokuhyou_value) > light_value) {
			return FALSE;
		}
		else {
			return TRUE;
		}
	}
	return TRUE;
}


//尻尾角度維持
void taildown(){

	static const float t_Kp = 0.7;

	static float t_hensa = 0;
	static float t_speed = 0;

	switch(tail_mode){
		case(RN_TAILDOWN):
			t_hensa = ANGLEOFDOWN - ecrobot_get_motor_rev(NXT_PORT_A);
			break;

		case(RN_TAILUP):
			t_hensa = ANGLEOFUP - ecrobot_get_motor_rev(NXT_PORT_A);
			break;

		case(RN_TAILPUSH):
			t_hensa = ANGLEOFPUSH - ecrobot_get_motor_rev(NXT_PORT_A);
			break;
		default:
			break;
	}

	t_speed = t_Kp*t_hensa;

		if (t_speed < -100)
			t_speed = -100;
		else if (t_speed > 100)
			t_speed = 100;

	ecrobot_set_motor_speed(NXT_PORT_A, t_speed);

}

//リモートスタート用関数
static int remote_start(void)
{
	int i;
	unsigned int rx_len;
	unsigned char start = 0;

	for (i=0; i<BT_MAX_RX_BUF_SIZE; i++)
	{
		rx_buf[i] = 0; /* 受信バッファをクリア */
	}

	rx_len = ecrobot_read_bt(rx_buf, 0, BT_MAX_RX_BUF_SIZE);
	if (rx_len > 0)
	{
		/* 受信データあり */
		if (rx_buf[0] == CMD_START)
		{
			start = 1; /* 走行開始 */
		}
		ecrobot_send_bt(rx_buf, 0, rx_len); /* 受信データをエコーバック */
	}

	return start;
}

//ライン無視走行信号受信用関数
static int RN_mandatory_run_remotestart(void)
{
	int i;
	unsigned int rx_len;
	unsigned char start = 0;

	for (i=0; i<BT_MAX_RX_BUF_SIZE; i++)
	{
		rx_buf[i] = 0; /* 受信バッファをクリア */
	}

	rx_len = ecrobot_read_bt(rx_buf, 0, BT_MAX_RX_BUF_SIZE);
	if (rx_len > 0)
	{
		/* 受信データあり */
		if (rx_buf[0] == MANDATORY_START_RUN_SIGNAL)
		{
			start = 1; /* 走行開始 */
		}
		ecrobot_send_bt(rx_buf, 0, rx_len); /* 受信データをエコーバック */
	}

	return start;
}


//走行状態切り替え
void RN_run_state_change()
{
	//static float beforestop = 0;

	switch (setting_mode){
			//キャリブレーション
		case (RN_SETTINGMODE_START):
			RN_calibrate();  //after calibarate cahge to RU_RUN
			break;

			//通常走行
		case (RN_RUN):
			RA_linetrace_PID(20); //通常走行用関数
			checkVariation();
			
			/*
			revL = nxt_motor_get_count(NXT_PORT_C);
			revR = nxt_motor_get_count(NXT_PORT_B);
			distance_before = fabs(CIRCUMFERENCE/360.0 * ((revL+revR)/2.0));
			*/
			//PCからの信号を受信（この場合'2'）を受信したら状態遷移
			if(RN_mandatory_run_remotestart()==1){
				setting_mode = RN_MANDATORY_LINE_OUT;
			
			}
			break;

			//強制停止
		case(RN_STOP):
			RN_stop();
			break;

	
		case(RN_MANDATORY_LINE_OUT):
			RN_mandatory_line_out(30,0);
			break;

		default:
			break;
	}
}

void RN_stop(){
	cmd_turn=0;
	cmd_forward=0;

}

void initialze_PID_param(){

	hensa = 0;
	i_hensa =0;
	d_hensa =0;
	bf_hensa =0;
}

//キャリブレーション
void RN_calibrate()
{

	//黒値
	while(1){
		if(ecrobot_get_touch_sensor(NXT_PORT_S4) == TRUE)
		{
			ecrobot_sound_tone(880, 512, 30);
			black_value=ecrobot_get_light_sensor(NXT_PORT_S3);
			systick_wait_ms(500);
			break;
		}
	}

	//白値
	while(1){
		if(ecrobot_get_touch_sensor(NXT_PORT_S4) == TRUE)
		{
			ecrobot_sound_tone(906, 512, 30);
			white_value=ecrobot_get_light_sensor(NXT_PORT_S3);
			systick_wait_ms(500);
			break;
		}
	}

	//灰色値計算
	gray_value=(black_value+white_value)/2;
	//ライントレース目標値の計算
	line_mokuhyou_value =(black_value*0.6+white_value*0.4);

	//ジャイロオフセット及びバッテリ電圧値
	while(1){
		if(ecrobot_get_touch_sensor(NXT_PORT_S4) == TRUE)
		{
			ecrobot_sound_tone(932, 512, 30);
			gyro_offset += (U32)ecrobot_get_gyro_sensor(NXT_PORT_S1);
			battery_value = ecrobot_get_battery_voltage();
			min_vol = battery_value;
			systick_wait_ms(500);
			break;
		}
	}

	//走行開始合図
	while(1){

		//リモートスタート
		if(remote_start()==1)
		{
			ecrobot_sound_tone(982,512,30);
			tail_mode = RN_TAILUP;
			setting_mode = RN_RUN;
			runner_mode = RN_MODE_CONTROL; //バランサー起動
			break;
		}

		//タッチセンサスタート
		else if (ecrobot_get_touch_sensor(NXT_PORT_S4) == TRUE)
		{
			ecrobot_sound_tone(982,512,30);

			while(1){
					if (ecrobot_get_touch_sensor(NXT_PORT_S4) != TRUE)
					{
						setting_mode = RN_RUN;
						runner_mode = RN_MODE_CONTROL; //バランサー起動
						tail_mode = RN_TAILUP;
						break;
					}
				}
			break;
		}
	}

	//キャリブレーション終了

}


//走行方法設定
void RN_modesetting()
{
	switch (runner_mode){
		case (RN_MODE_INIT):
			cmd_forward = 0;
			cmd_turn = 0;
			break;

			//バランサー
		case (RN_MODE_CONTROL):
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

		case (RN_MODE_STOP):
			break;

		default:
			nxt_motor_set_speed(NXT_PORT_C, 0, 1);
			nxt_motor_set_speed(NXT_PORT_B, 0, 1);
			break;
	}
}

//ラインを無視した走行をし、１秒経過したら通常走行へ戻る。
void RN_mandatory_line_out(U8 port_B_speed,U8 port_C_speed){
	
	/*
	static int flag =0;
	if(flag ==0) {
	nxt_motor_set_speed(NXT_PORT_C,port_C_speed,1);
	nxt_motor_set_speed(NXT_PORT_B,port_B_speed,1);
	}
	else {
		setting_mode = RN_RUN; //PIDを用いた通常走行へ戻る。
	
	}
	*/
	

	//nxt_motor_set_speed(NXT_PORT_C,port_C_speed,1);
	//nxt_motor_set_speed(NXT_PORT_B,port_B_speed,1);
	cmd_turn=port_B_speed;
	if (TU_is_start()/* && flag*/ ==0) {
		/** 4秒のタイマ監視開始 */
	/*
		ecrobot_sound_tone(880, 512, 30);
	systick_wait_ms(300); //音を鳴らす
	*/
		TU_start(1000); 
	}

	if (TRUE == TU_is_timeout()) {
	//balance_init();						/* 倒立振子制御初期化 */
	//nxt_motor_set_count(NXT_PORT_C, 0); /* 左モータエンコーダリセット */
	//nxt_motor_set_count(NXT_PORT_B, 0); /* 右モータエンコーダリセット */
	//initialze_PID_param();
	TU_finish(); /** タイマ監視を終了させる */ 	
	//flag =1; //一回タイマを使ったらもう使わない。
	/*
	ecrobot_sound_tone(440, 512, 30);
	systick_wait_ms(500); //音を鳴らす
	*/
	setting_mode = RN_RUN; //PIDを用いた通常走行へ戻る。
	
		
	}
		
	
}


void rapid_speed_up(){


}

//走行方法管理(4ms)
TASK(ActionTask)
{
	RN_modesetting();
	taildown();
	TerminateTask();
}

//走行状態管理(5ms)
TASK(ActionTask2)
{
	RN_run_state_change();
	TerminateTask();
}

//状態表示管理(20ms)
TASK(DisplayTask)
{
	ecrobot_status_monitor(target_subsystem_name);
	TerminateTask();
}

//ログ送信管理(50ms)
TASK(LogTask)
{
	logSend(cmd_forward,cmd_turn,ecrobot_get_battery_voltage(),getLightSensorVarieation()/*min_vol*/,
			distance_before - distance_slow,ecrobot_get_gyro_sensor(NXT_PORT_S1));
	TerminateTask();
}


/******************************** END OF FILE ********************************/

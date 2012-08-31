/*
 *	PID_parameter.c
 *	PIDパラメータ調節用プログラム
 */

#include "PID_parameter.h"
#include "logSend.h"
#include "math.h"
#include "tyreal_light_ver.h"


/*
 *	各種変数定義
 */

//ライントレース用目標値
static unsigned int BLACK_VALUE;	//黒値
static unsigned int WHITE_VALUE;	//白値
static unsigned int GRAY_VALUE;		//灰色値（現在は黒と白の平均値）

static int counter = 0;

//尻尾設定角度
#define ANGLEOFDOWN 95			//降下目標角度
#define ANGLEOFUP 0					//上昇目標角度

#define CMD_STOP '3'

#define ANGLE_OF_AIM 180  //右を向く角度

//速度調節係数
#define SPEED_COUNT 10

//ライントレース時PID制御用係数

static float Kp = 0.436;//0.436;				//P制御用
static float Ki = 0;//1.31;					//I制御用
static float Kd = 0.074;//0.011;				//D制御用

//ジャイロセンサオフセット計算用変数
static U32	gyro_offset = 0;    /* gyro sensor offset value */ 

char rx_buf[BT_MAX_RX_BUF_SIZE];

/* バランスコントロールへ渡すコマンド用変数 */
S8  cmd_forward, cmd_turn;
/* バランスコントロールから返されるモータ制御用変数 */
S8	pwm_l, pwm_r;

/*
 *	状態定義
 */

//システム全体の状態
typedef enum{
	RN_MODE_INIT, 					//初期状態
	RN_MODE_BALANCE,				//倒立制御ON
	RN_MODE_BALANCEOFF,				//倒立制御OFF
} RN_MODE;


typedef enum{
	RN_TYREAL,
	RN_SETTINGMODE_START,		//初期状態
	RN_RUN,						//基本走行（ライントレース）
	RN_RIGHT,
} RN_SETTINGMODE;


//尻尾の状態
typedef enum{
	RN_TAILDOWN,				//尻尾降下
	RN_TAILUP,					//尻尾上昇
} RN_TAILMODE;


//初期状態
RN_MODE runner_mode = RN_MODE_INIT;
RN_SETTINGMODE setting_mode = RN_SETTINGMODE_START;
//RN_SETTINGMODE setting_mode = RN_TYREAL;

RN_TAILMODE tail_mode = RN_TAILDOWN;

/*	
 *	各種関数定義
 */

//各種プライベート関数
void RN_calibrate();
void RN_setting();
int online();
void RA_linetrace(int forward_speed, int turn_speed);
void RA_linetrace_PID(int forward_speed);
void tailcontrol();
int RA_speed(int forward_speed);
void RN_modesetting();
static int remote_start(void);
static int remote_stop(void);
static int right(void);

//カウンタの宣言
DeclareCounter(SysTimerCnt);

//液晶ディスプレイに表示するシステム名設定
const char target_subsystem_name[] = "PID";


/*
 *	各種関数
 */

//初期処理
void ecrobot_device_initialize(void)
{
	ecrobot_set_light_sensor_active(NXT_PORT_S3);
	ecrobot_init_bt_slave("LEJOS-OSEK");

	ecrobot_set_motor_rev(NXT_PORT_A,0);
	ecrobot_set_motor_rev(NXT_PORT_B,0);
	ecrobot_set_motor_rev(NXT_PORT_C,0);
}


//後始末処理
void ecrobot_device_terminate(void)
{
	tail_mode = RN_TAILUP;
	ecrobot_set_motor_speed(NXT_PORT_A, 0);
	ecrobot_set_motor_speed(NXT_PORT_B, 0);
	ecrobot_set_motor_speed(NXT_PORT_C, 0);

	ecrobot_set_light_sensor_inactive(NXT_PORT_S3);
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


//PID制御ライントレース関数
void RA_linetrace_PID(int forward_speed) {

	//PID制御用偏差値
	static float hensa;					//P制御用
	static float i_hensa = 0;			//I制御用
	static float d_hensa = 0;			//D制御用
	static float bf_hensa = 0;
	float right_motor_turn=0,left_motor_turn;
	
	forward_speed = RA_speed(forward_speed);	//速度を段階的に変化

	if(forward_speed > 0)
		hensa = (float)GRAY_VALUE - (float)ecrobot_get_light_sensor(NXT_PORT_S3);
	else
		hensa = (float)ecrobot_get_light_sensor(NXT_PORT_S3) - (float)GRAY_VALUE;

	i_hensa = i_hensa+(hensa*0.0002);
	d_hensa = (hensa - bf_hensa)/0.0002;
	bf_hensa = hensa;

	cmd_turn = -(Kp * hensa + Ki * i_hensa + Kd * d_hensa);
	
//	cmd_turn = -(Kp * hensa);

	right_motor_turn = forward_speed - cmd_turn/2;
	left_motor_turn = forward_speed  + cmd_turn/2;

	if (-128 > right_motor_turn) {
		right_motor_turn = -128;
	} else if (127 < right_motor_turn) {
		right_motor_turn = 127;
	}

	if (-128 > left_motor_turn) {
		left_motor_turn = -128;
	} else if (127 < left_motor_turn) {
		left_motor_turn = 127;
	}

	pwm_l = left_motor_turn;
	pwm_r = right_motor_turn;

}


//段階的加速用関数（指定量だけ速度を徐々に上昇）
int RA_speed(int forward_speed){

	static int result_speed = 0;
	counter++;

	if(counter >= SPEED_COUNT)
	{
		if(forward_speed - result_speed >= 0){
			result_speed++;

			if(result_speed > forward_speed)
				result_speed = forward_speed;
		}
		else{
			result_speed--;

			if(result_speed < forward_speed)
				result_speed = forward_speed;
		}
		counter = 0;
	}

	return result_speed;
}

//尻尾角度コントロール関数
void tailcontrol(){

	static const float t_Kp = 1.85;

	static float t_hensa = 0;
	static float t_speed = 0;

	switch(tail_mode){
		case(RN_TAILDOWN):
			t_hensa = ANGLEOFDOWN - ecrobot_get_motor_rev(NXT_PORT_A);		//尻尾を下げる
			break;

		case(RN_TAILUP):
			t_hensa = ANGLEOFUP - ecrobot_get_motor_rev(NXT_PORT_A);		//尻尾を上げる
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


//走行設定関数
void RN_setting()
{
	int speed = 90;

	switch (setting_mode){

		case (RN_TYREAL):
			do_tyreal(&Kp,&Ki,&Kd);

			if(ecrobot_get_touch_sensor(NXT_PORT_S4) == TRUE)
			{
							ecrobot_set_motor_speed(NXT_PORT_A,0);
			ecrobot_set_motor_speed(NXT_PORT_B,0);
			ecrobot_set_motor_speed(NXT_PORT_C,0);
				ecrobot_sound_tone(932, 512, 20);
				systick_wait_ms(100);
				ecrobot_sound_tone(466, 256, 20);
				systick_wait_ms(10);
				systick_wait_ms(500);
				setting_mode = RN_SETTINGMODE_START;
			}
			break;
		
			//走行開始前
		case (RN_SETTINGMODE_START):
			RN_calibrate();								//キャリブレーション
			ecrobot_set_motor_speed(NXT_PORT_A,0);
			ecrobot_set_motor_speed(NXT_PORT_B,0);
			ecrobot_set_motor_speed(NXT_PORT_C,0);
			break;

			//通常走行
		case (RN_RUN):
			RA_linetrace_PID(speed);
			if (remote_stop()==1)
			{	
				ecrobot_sound_tone(932, 512, 20);
				systick_wait_ms(100);
				ecrobot_sound_tone(466, 256, 20);
				systick_wait_ms(500);
 		 		//speed = 0;
				runner_mode = RN_MODE_BALANCEOFF;
				setting_mode = RN_TYREAL;
				
			}
			break;


			//右を向く
		case(RN_RIGHT):
			right();
			break;

		default:
			break;
	}
}

//右を向く関数
static int right()
{
	ecrobot_set_motor_rev(NXT_PORT_C, 0);
	ecrobot_set_motor_speed(NXT_PORT_C, 50);
	
	while(ecrobot_get_motor_rev(NXT_PORT_C) <= ANGLE_OF_AIM)
	{
	}
	
	ecrobot_set_motor_speed(NXT_PORT_C, 0);
}



//キャリブレーション関数
void RN_calibrate()
{

	//黒値
	while(1){
		if(ecrobot_get_touch_sensor(NXT_PORT_S4) == TRUE)
		{
			ecrobot_sound_tone(880, 512, 10);
			BLACK_VALUE=ecrobot_get_light_sensor(NXT_PORT_S3);
			systick_wait_ms(500);
			break;
		}
	}

	//白値
	while(1){
		if(ecrobot_get_touch_sensor(NXT_PORT_S4) == TRUE)
		{
			ecrobot_sound_tone(906, 512, 10);
			WHITE_VALUE=ecrobot_get_light_sensor(NXT_PORT_S3);
			systick_wait_ms(500);
			break;
		}
	}

	//灰色値計算
	GRAY_VALUE=(BLACK_VALUE+WHITE_VALUE)/2;

	//走行開始合図
	while(1){

		//タッチセンサスタート
		if (ecrobot_get_touch_sensor(NXT_PORT_S4) == TRUE)
		{
			ecrobot_sound_tone(982,512,10);

			while(1){
					if (ecrobot_get_touch_sensor(NXT_PORT_S4) != TRUE)
					{
						setting_mode = RN_RUN;
						runner_mode = RN_MODE_BALANCEOFF;
						break;
					}
				}
			break;
		}
	}

}
//リモートストップ関数
static int remote_stop(void)
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
		if (rx_buf[0] == CMD_STOP)
		{
			start = 1; /* 走行停止 */
		}
		ecrobot_send_bt(rx_buf, 0, rx_len); /* 受信データをエコーバック */
	}

	return start;
}

//走行体状態設定関数
void RN_modesetting()
{
	switch (runner_mode){

			//初期状態
		case (RN_MODE_INIT):
			cmd_forward = 0;
			cmd_turn = 0;
			break;

			//バランサーON
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

		case (RN_MODE_BALANCEOFF):
			nxt_motor_set_speed(NXT_PORT_C, pwm_l, 1);
			nxt_motor_set_speed(NXT_PORT_B, pwm_r, 1);
			break;

		default:
			break;
	}
}

/*
 *	各種タスク
 */

//走行方法管理(4ms)
TASK(ActionTask)
{
	RN_modesetting();	//走行体状態
	tailcontrol();		//尻尾コントロール
	TerminateTask();
}

//走行状態管理(5ms)
TASK(ActionTask2)
{
	RN_setting();		//走行状態
	TerminateTask();
}

//ログ送信管理(50ms)
TASK(LogTask)
{
	logSend(cmd_forward,cmd_turn,pwm_l,pwm_r,0,0);		//ログ取り
	TerminateTask();
}


/******************************** END OF FILE ********************************/

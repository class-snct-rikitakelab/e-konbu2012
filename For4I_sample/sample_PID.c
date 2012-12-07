#include "sample_PID.h"
#include "math.h"

/*
 *	各種変数定義
 */

//ライントレース用目標値
static unsigned int BLACK_VALUE;	//黒値
static unsigned int WHITE_VALUE;	//白値
static unsigned int GRAY_VALUE;		//灰色値（現在は黒と白の平均値）

//ジャイロオフセット値
static unsigned int GYRO_OFFSET;	

static int counter = 0;

//尻尾設定角度
#define ANGLEOFDOWN 95			//降下目標角度
#define ANGLEOFUP 0				//上昇目標角度

//速度調節係数
#define SPEED_COUNT 20

//PID制御用偏差値
static float hensa;					//P制御用
static float i_hensa = 0;			//I制御用
static float d_hensa = 0;			//D制御用
static float bf_hensa = 0;

//ライントレース時PID制御用係数
static float Kp = 1.360;			//P制御用
static float Ki = 2.6;				//I制御用
static float Kd = 0.003;			//D制御用

//ジャイロセンサオフセット計算用変数
static U32	gyro_offset = 0;    /* gyro sensor offset value */ 

//Bluetooth通信用バッファ
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
	RN_SETTINGMODE_START,		//初期状態
	RN_RUN,						//基本走行（ライントレース）
	RN_STOP,					//停止
} RN_SETTINGMODE;


//尻尾の状態
typedef enum{
	RN_TAILDOWN,				//尻尾降下
	RN_TAILUP,					//尻尾上昇
} RN_TAILMODE;


//初期状態
RN_MODE runner_mode = RN_MODE_INIT;
RN_SETTINGMODE setting_mode = RN_SETTINGMODE_START;
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
void RA_speed(int limit,int s_Kp);
void RN_modesetting();
static int remote_start(void);

//カウンタの宣言
DeclareCounter(SysTimerCnt);

//液晶ディスプレイに表示するシステム名設定
const char target_subsystem_name[] = "sample";


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


	RA_speed(forward_speed,2);	//速度を段階的に変化

	if(forward_speed > 0)
		hensa = (float)GRAY_VALUE - (float)ecrobot_get_light_sensor(NXT_PORT_S3);
	else
		hensa = (float)ecrobot_get_light_sensor(NXT_PORT_S3) - (float)GRAY_VALUE;

	i_hensa = i_hensa+(hensa*0.0005);
	d_hensa = (hensa - bf_hensa)/0.0005;
	bf_hensa = hensa;

	cmd_turn = -(Kp * hensa + Ki * i_hensa + Kd * d_hensa);

	if (-100 > cmd_turn) {
		cmd_turn = -100;
	} else if (100 < cmd_turn) {
		cmd_turn = 100;
	}

	/*倒立制御OFF時*/
	nxt_motor_set_speed(NXT_PORT_C, forward_speed + cmd_turn/2, 1);
	nxt_motor_set_speed(NXT_PORT_B, forward_speed - cmd_turn/2, 1);

}

//段階的加速用関数（指定量だけ速度を徐々に上昇）
void RA_speed(int limit,int s_Kp){

	static int forward_speed;

	counter ++;

	if(counter >= SPEED_COUNT)
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

//尻尾角度コントロール関数
void tailcontrol(){

	static const float t_Kp = 1.7;

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
	static int speed = 30;   //速度
	
	switch (setting_mode){

		//走行開始前
		case (RN_SETTINGMODE_START):
			RN_calibrate();				//キャリブレーション
			break;

		//通常走行
		case (RN_RUN):
			RA_linetrace_PID(speed);	//PID制御によるライントレース
			break;

		default:
			break;

		}
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

	//ジャイロオフセット及びバッテリ電圧値
	while(1){
		if(ecrobot_get_touch_sensor(NXT_PORT_S4) == TRUE)
		{
			ecrobot_sound_tone(932, 512, 10);
			gyro_offset += (U32)ecrobot_get_gyro_sensor(NXT_PORT_S1);
			GYRO_OFFSET = gyro_offset;
			systick_wait_ms(500);
			break;
		}
	}

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

//状態表示管理(20ms)
TASK(DisplayTask)
{
	TerminateTask();
}

//ログ送信管理(50ms)
TASK(LogTask)
{
	TerminateTask();
}


/******************************** END OF FILE ********************************/

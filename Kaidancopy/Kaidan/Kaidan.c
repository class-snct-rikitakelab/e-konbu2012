

#include "Kaidan.h"
#include "logSend.h"
//#include "tyreal.h"
#include "math.h"


/*
 *	各種変数定義
 */

//ライントレース用目標値
static unsigned int BLACK_VALUE;	//黒値
static unsigned int WHITE_VALUE;	//白値
static unsigned int GRAY_VALUE;		//灰色値（現在は黒と白の平均値）

static int counter = 0;


//尻尾設定角度
#define ANGLEOFDOWN 104			//降下目標角度
#define ANGLEOFUP 0					//上昇目標角度

//速度調節係数
#define SPEED_COUNT 50

//バッテリ降下値
#define DOWN_BATTERY 450			//バッテリ降下値

//ジャイロ振幅値
#define PM_GYRO 65

//車輪の円周[cm]
#define CIRCUMFERENCE 25.8			//車輪の円周

#define CMD_START '1'    			//リモートスタートコマンド(変更禁止)

#define POSITION_X0 0
#define POSITION_Y0 0
#define THETA_0 0

#define WHEEL_R		41
#define MACHINE_W	162

//PID制御用偏差値
static float hensa;					//P制御用
static float i_hensa = 0;			//I制御用
static float d_hensa = 0;			//D制御用
static float bf_hensa = 0;


//ライントレース時PID制御用係数

static float Kp = 1.85;				//P制御用
static float Ki = 2.6;				//I制御用
static float Kd = 0.003;				//D制御用


static int wait_count = 0;

static double min_vol;
static int stepflag=0;

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
float distance_before_step = 0;
float distance_step_brake = 0;
float distance_step_stop = 0;


/* 自己位置同定用　変数宣言 */
float d_theta_r;					/* 現在の右モータ回転角度 [rad] */
float d_theta_l;					/* 現在の左モータ回転角度 [rad] */
static float d_theta_r_t;			/* 1 ステップ前の右モータ回転角度 [rad] */
static float d_theta_l_t;			/* 1 ステップ前の左モータ回転角度 [rad] */
float velocity_r;					/* 右車輪移動速度 [cm/s] */
float velocity_l;					/* 左車輪移動速度 [cm/s] */
float velocity;						/* ロボットの移動速度 [cm/s] */
float omega;						/* ロボットの回転角角度 [rad/s] */
static float position_x = POSITION_X0; /* ロボットの x 座標 */
static float position_y = POSITION_Y0; /* ロボットの y 座標 */
static float theta = THETA_0;		/* ロボットの姿勢角 */
unsigned short int l_val;			/* 光センサ値 */
int temp_x;							/* ロボットの x 座標（出力処理用） */
int temp_y;							/* ロボットの y 座標（出力処理用） */
static double omega_r;			//右車輪の回転角速度
static double omega_l;			//左車輪の回転角速度
unsigned char tx_buf[BT_MAX_TX_BUF_SIZE]; /* 送信バッファ */

/*
 *	状態定義
 */

//システム全体の状態
typedef enum{
	RN_MODE_INIT, 					//初期状態
	RN_MODE_CONTROL,				//倒立制御ON
	RN_MODE_STOP,					//倒立制御OFF
	RN_MODE_BALANCE,				//倒立制御ON
	RN_MODE_BALANCEOFF,					//倒立制御OFF
} RN_MODE;


typedef enum{
	RN_SETTINGMODE_START,		//初期状態
	RN_RUN,						//基本走行（ライントレース）
	RN_SLOW_RUN,				//低速走行
	RN_STOP,						//停止
	RN_STOP_WAIT,
	RN_RUPID_SPEED_UP,
	RN_STEP_BRAKE,				//ブレーキ
	RN_STEP_STOP,				//停止
	TYREAL,
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


//段差検知関連マクロ、プロトタイプ
#define RUPID_SPEED_UP_SIGNAL 3
static int RN_rupid_speed_up_signal_recevie(void);

/*	
 *	各種関数定義
 */

//各種プライベート関数
void RN_calibrate();
void RN_setting();
int online();
void RA_linetrace(int forward_speed, int turn_speed);
void RA_linetrace_PID(int forward_speed);

int shock();
void tailcontrol();
void RA_linetrace_P(int forward_speed);
void RA_speed(int limit,int s_Kp);
int RA_wheels(int turn);
void RN_modesetting();
static int remote_start(void);
void rupid_speed_up(int target_forward_speed);
void self_location(void);

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
const char target_subsystem_name[] = "Kaidan";



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
	ecrobot_set_motor_speed(NXT_PORT_A,0);
	ecrobot_set_motor_speed(NXT_PORT_B,0);
	ecrobot_set_motor_speed(NXT_PORT_C,0);
}


//後始末処理
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


//ON-OFF制御ライントレース関数
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


//P制御ライントレース関数
void RA_linetrace_P(int forward_speed){

	cmd_forward = forward_speed;

	hensa = (float)GRAY_VALUE - (float)ecrobot_get_light_sensor(NXT_PORT_S3);

	cmd_turn = -(1.4 * hensa);
	if (-100 > cmd_turn) {
		cmd_turn = -100;
	} else if (100 < cmd_turn) {
		cmd_turn = 100;
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
	//nxt_motor_set_speed(NXT_PORT_C, forward_speed + cmd_turn/2, 1);
	//nxt_motor_set_speed(NXT_PORT_B, forward_speed - cmd_turn/2, 1);

}

//PID制御用偏差リセット関数
void RA_hensareset(void)
{
	hensa = 0;
	i_hensa = 0;
	d_hensa = 0;
	bf_hensa = 0;
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


//車輪回転量差調節関数（PID制御）
int RA_wheels(int turn){
	float w_kp = 1.4;

	signed long def = ecrobot_get_motor_rev(NXT_PORT_B) - ecrobot_get_motor_rev(NXT_PORT_C);
	turn = w_kp * def;

	return turn;
}


//衝撃検知関数
int shock(void){
	int result=0;
	//電圧降下の最小値を更新
	if(min_vol>ecrobot_get_battery_voltage())
		min_vol=ecrobot_get_battery_voltage();

	//ジャイロ及び電圧降下から衝撃検知
	if((ecrobot_get_gyro_sensor(NXT_PORT_S1) <= gyro_offset-PM_GYRO ||ecrobot_get_gyro_sensor(NXT_PORT_S1) >= gyro_offset+PM_GYRO) 
		&& min_vol <= battery_value-DOWN_BATTERY)
	{
		ecrobot_sound_tone(880,512,30);

		revL = nxt_motor_get_count(NXT_PORT_C);
		revR = nxt_motor_get_count(NXT_PORT_B);

		distance_before_step = fabs(CIRCUMFERENCE/360.0 * ((revL+revR)/2.0));	//段差突入時の距離を測定


	//	setting_mode = RN_SLOW_RUN; 
		
		stepflag = 1;
		result = 1;
		//setting_mode = RN_STEP_BRAKE;		//階段へ向かいブレーキをかける

		min_vol = battery_value;			//最小値リセット
	}
	return result;
}


//ON-OFF制御用ライン判定関数
int online(void) {

	int light_value;
	light_value = ecrobot_get_light_sensor(NXT_PORT_S3);

	if (GRAY_VALUE > light_value) {
		if ((GRAY_VALUE) > light_value) {
			return FALSE;
		}
		else {
			return TRUE;
		}
	}
	return TRUE;
}


//尻尾角度コントロール関数
void tailcontrol(){

	static const float t_Kp = 0.7;

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

//リモートスタート関数
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



//走行設定関数
void RN_setting()
{
//	static float beforestop = 0;

	switch (setting_mode){
			//走行開始前
		case (RN_SETTINGMODE_START):
			RN_calibrate();				//キャリブレーション
			break;

			//通常走行
		case (RN_RUN):
			RA_linetrace_PID(25);
			wait_count = 0;
			if(remote_start()==1)
			{
				gyro_offset += 17;				
				/*while(1)
				{
					RA_linetrace_PID(25);
					wait_count++;
					if(wait_count == 400){
						gyro_offset -= 100;
						tail_mode = RN_TAILDOWN;
					}
					if(wait_count > 500)
					{
						setting_mode = RN_STOP;
						tail_mode = RN_TAILDOWN;
						runner_mode = RN_MODE_BALANCEOFF;
						break;
					}
				}
				*/
			}



			/*
			if(cmd_forward < 50)
				RA_speed(50,1);
			cmd_turn = RA_wheels(cmd_turn);
			*/
			/*
			if(1==RN_rupid_speed_up_signal_recevie()){
			ecrobot_sound_tone(880, 512, 30);
			systick_wait_ms(500);
			setting_mode = RN_RUPID_SPEED_UP;
			}

			//一回だけ段差検知
			if(stepflag == 0){
			if(shock()==1){
				ecrobot_sound_tone(180, 512, 30);
			systick_wait_ms(500);
			
				setting_mode = RN_STOP;
			}
			}
			*/
			/*
			if ( tyreal_trigger() == 1) {
				ecrobot_sound_tone(932, 512, VOL);
				systick_wait_ms(100);
				ecrobot_sound_tone(466, 256, VOL);
				systick_wait_ms(10);
				setting_mode = TYREAL;
			}
			*/

			/*

			RA_linetrace_PID(45);		//ライントレース
			shock();					//段差検知
			*/
			break;

			//一定距離分ブレーキ
		case(RN_STEP_BRAKE):
			revL = nxt_motor_get_count(NXT_PORT_C);
			revR = nxt_motor_get_count(NXT_PORT_B);

			distance_step_brake = fabs(CIRCUMFERENCE/360.0 * ((revL+revR)/2.0));	//現在の走行距離を計測

			RA_speed(-30,1);								//徐々にブレーキを強くしていく（前の状態の速度のままでは止まって欲しい位置で止まってくれないため、ブレーキを強くする）
			cmd_turn = RA_wheels(cmd_turn);					//両車輪の回転量を同じにして、方向が余計にずれるのを防ぐ

			//一定距離検知後、停止モードへ
			if((distance_before_step - distance_step_brake <= -10))	//-10はNXT Communicatorのデータから判断
			{
				balance_init();										//バランサーを初期化
				nxt_motor_set_count(NXT_PORT_B,0);					//モータの回転量を初期化
				nxt_motor_set_count(NXT_PORT_C,0);
				setting_mode = RN_STEP_STOP;						//停止状態へ
			}

			break;


			//停止状態
		case(RN_STEP_STOP):
			revL = nxt_motor_get_count(NXT_PORT_C);
			revR = nxt_motor_get_count(NXT_PORT_B);
			distance_step_stop = fabs(CIRCUMFERENCE/360.0 * ((revL+revR)/2.0));	//現在の走行距離測定

			wait_count++;			//時間カウント
			RA_linetrace(0,2);		//ON/OFF制御の速度0ライントレースで位置を整える

			/*
			//一定距離走行後、通常走行へ
			if(distance_step_stop-distance_step_brake < 0)
			{
				balance_init();
				nxt_motor_set_count(NXT_PORT_B,0);
				nxt_motor_set_count(NXT_PORT_C,0);
				RA_hensareset();
				wait_count = 0;
				setting_mode = RN_RUN;
				tail_mode = RN_TAILUP;
			}
			*/

			//一定時間経過後通常走行へ
			if(wait_count >= 550)
			{
				balance_init();						//バランサー初期化
				nxt_motor_set_count(NXT_PORT_B,0);	//モータの回転量初期化
				nxt_motor_set_count(NXT_PORT_C,0);
				RA_hensareset();					//ライントレースの偏差初期化
				wait_count = 0;						//カウンタ初期化
				setting_mode = RN_RUN;				//通常走行に移行
				tail_mode = RN_TAILUP;				//尻尾を上げる
			}

			break;

			//強制停止
		case(RN_STOP):
		/*
			tailcontrol();
			 tail_mode = RN_TAILDOWN;
			*/ 
			
			nxt_motor_set_speed(NXT_PORT_C, 0, 1);
			nxt_motor_set_speed(NXT_PORT_B, 0, 1);
			/*
			RA_linetrace_PID(3);
			cmd_turn = RA_wheels(cmd_turn);
			*/
			/*
			cmd_turn=0;
			cmd_forward=0;
			*/
			/*
			if(1==RN_rupid_speed_up_signal_recevie()){
			ecrobot_sound_tone(880, 512, 30);
			systick_wait_ms(500);
			setting_mode = RN_RUPID_SPEED_UP;
			}
			*/
			break;
		case (RN_RUPID_SPEED_UP):
			rupid_speed_up(80);
			ecrobot_sound_tone(880,512,15);
			/*
		case(TYREAL):
			do_tyreal();
			break;
			*/
		default:
			break;
	}
}

//急加速用関数
void rupid_speed_up(int target_forward_speed){
	static int rupid_speed_up_counter=0;
	int gyro_offset_operation = 10;
	
	rupid_speed_up_counter++;
	cmd_turn = RA_wheels(cmd_turn);

	if(rupid_speed_up_counter<2){
	gyro_offset = gyro_offset + gyro_offset_operation;
	//	cmd_forward++;
		//gyro_offset = gyro_offset - 0.5;
	}
	else if(rupid_speed_up_counter==100){
	//rupid_speed_up_counter=0;
	gyro_offset = gyro_offset-gyro_offset_operation;
		//setting_mode = RN_STOP;
	}
	if(rupid_speed_up_counter>2 && rupid_speed_up_counter<150 ) {
	RA_linetrace_PID(25);
	
	}
	//RA_linetrace_PID(25/*target_forward_speed*/);
	if(rupid_speed_up_counter>150) {
	RA_linetrace_PID(50/*target_forward_speed*/);
	
	}
	if(rupid_speed_up_counter>200) {
	RA_linetrace_PID(50/*target_forward_speed*/);
	rupid_speed_up_counter=0;
	setting_mode = RN_STOP;
	}

	/*
	int forward_hensa;

	forward_hensa = target_forward_speed - cmd_forward;
	

	cmd_forward = Kp*forward_hensa;

	if (cmd_forward < -100) {
		cmd_forward = -100;
	}
	else if (cmd_forward > 100) {
		cmd_forward = 100;
	}
	*/


	/*
	limit が目標値
	s_Kpは一回あたりに加減する操作量？
	static int forward_speed;

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
	*/

}

static int RN_rupid_speed_up_signal_recevie(void)
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
		if (rx_buf[0] == RUPID_SPEED_UP_SIGNAL)
		{
			start = 1;
		}
		
		ecrobot_send_bt(rx_buf, 0, rx_len); /* 受信データをエコーバック */
	}
	else if(ecrobot_get_touch_sensor(NXT_PORT_S4) == TRUE)
	{
		
			start = 1; 

	}

	return start;
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
			ecrobot_sound_tone(982,512,10);
			tail_mode = RN_TAILUP;
			setting_mode = RN_RUN;
			runner_mode = RN_MODE_BALANCE;
			break;
		}

		//タッチセンサスタート
		else if (ecrobot_get_touch_sensor(NXT_PORT_S4) == TRUE)
		{
			ecrobot_sound_tone(982,512,10);

			while(1){
					if (ecrobot_get_touch_sensor(NXT_PORT_S4) != TRUE)
					{
						setting_mode = RN_RUN;
						runner_mode = RN_MODE_BALANCE;
						tail_mode = RN_TAILUP;
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


		case (RN_MODE_STOP):
			//バランサーOFF
		case (RN_MODE_BALANCEOFF):
			break;

		default:
			nxt_motor_set_speed(NXT_PORT_C, 0, 1);
			nxt_motor_set_speed(NXT_PORT_B, 0, 1);
			break;
	}
}

void self_location()
{
	d_theta_l = (float)nxt_motor_get_count(NXT_PORT_C) * M_PI / 180.0;
	d_theta_r = (float)nxt_motor_get_count(NXT_PORT_B) * M_PI / 180.0;

	omega_l = (d_theta_l - d_theta_l_t)/0.004;
	omega_r = (d_theta_r - d_theta_r_t)/0.004;

	velocity_l = (WHEEL_R * 0.1) * omega_l;
	velocity_r = (WHEEL_R * 0.1) * omega_r;

	velocity = (velocity_r + velocity_l) / 2.0;
	omega = (velocity_r - velocity_l) / (MACHINE_W * 0.1);

	d_theta_l_t = d_theta_l;
	d_theta_r_t = d_theta_r;

	theta += omega * 0.004 + THETA_0;
	position_x += velocity * cos(theta) * 0.004 + POSITION_X0;
	position_y += velocity * sin(theta) * 0.004 + POSITION_Y0;
	
}

/*
 *	各種タスク
 */

//走行方法管理(4ms)
TASK(ActionTask)
{
	RN_modesetting();	//走行体状態
	tailcontrol();		//尻尾コントロール
	self_location();	//自己位置同定
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
	ecrobot_status_monitor(target_subsystem_name);	//モニタ出力
	TerminateTask();
}

//ログ送信管理(50ms)
TASK(LogTask)
{
	logSend(cmd_forward,cmd_turn,position_x,position_y/*min_vol*/,
			velocity,ecrobot_get_gyro_sensor(NXT_PORT_S1));		//ログ取り
	TerminateTask();
}


/******************************** END OF FILE ********************************/

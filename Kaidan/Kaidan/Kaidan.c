

#include "Kaidan.h"
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

//ジャイロオフセット値
static unsigned int GYRO_OFFSET;	

static int counter = 0;


//尻尾設定角度
#define ANGLEOFDOWN 95			//降下目標角度
#define ANGLEOFUP 0					//上昇目標角度

//速度調節係数
#define SPEED_COUNT 50

//バッテリ降下値
#define STEP_BATTERY 300
#define STOP_BATTERY 400

//ジャイロ振幅値
#define PM_GYRO 65

//車輪の円周[cm]
#define CIRCUMFERENCE 25.8			//車輪の円周

#define CMD_START '1'    			//リモートスタートコマンド(変更禁止)
#define rapid_SPEED_UP_SIGNAL '3'

#define POSITION_X0 0
#define POSITION_Y0 0
#define THETA_0 0

#define WHEEL_R		41
#define MACHINE_W	162

#define RIGHT_ANGLE_LIGHT_VALUE 540		//ラインから脱した際の光センサの値(570)
#define RIGHT_ANGLE_AIM 140				//回転角度(160)

//PID制御用偏差値
static float hensa;					//P制御用
static float i_hensa = 0;			//I制御用
static float d_hensa = 0;			//D制御用
static float bf_hensa = 0;


static float Kp = 1.85;				//P制御用
static float Ki = 2.6;				//I制御用
static float Kd = 0.003;				//D制御用
//ライントレース時PID制御用係数



static int wait_count = 0;

static double min_vol;
static int stepflag = 0;

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
float distance_gyro_up = 0;
float distance_over_forty = 0;
float distance_turn_clear = 0;
float distance_turn_after = 0;


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

//バッテリ平均計算
int battery_average;
static int now_battery = 0;
static int bt_counter = 0;
static int before_battery =0;
static int average_flag;


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
	RN_STEP_RAPID,
	RN_STEP_SHOCK,
	RN_STEP_SLOW,
	RN_STEP_STAY,
	RN_STEP_SECOND,
	RN_STEP_TURN_LEFT,
	RN_STEP_TURN_FORWARD,
	RN_STEP_TURN_TAILUP,
	TYREAL
} RN_SETTINGMODE;


//尻尾の状態
typedef enum{
	RN_TAILDOWN,				//尻尾降下
	RN_TAILUP,					//尻尾上昇
} RN_TAILMODE;

//初期状態
RN_MODE runner_mode = RN_MODE_INIT;
RN_SETTINGMODE setting_mode = RN_SETTINGMODE_START;
//RN_SETTINGMODE setting_mode = TYREAL;
RN_TAILMODE tail_mode = RN_TAILDOWN;

//段差検知関連マクロ、プロトタイプ
static int RN_rapid_speed_up_signal_recevie(void);

/*	
 *	各種関数定義
 */

//各種プライベート関数
void RN_calibrate();
void RN_setting();
int online();
void RA_linetrace(int forward_speed, int turn_speed);
void RA_linetrace_PID(int forward_speed);

int shock(int target);
void tailcontrol();
void RA_linetrace_P(int forward_speed);
void RA_speed(int limit,int s_Kp);
int RA_wheels(int turn);
void RN_modesetting();
static int remote_start(void);
int rapid_speed_up(int target_gyro);
void self_location(void);
void battery_average_check(void);

//カウンタの宣言
DeclareCounter(SysTimerCnt);

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
	/*
	ecrobot_set_motor_speed(NXT_PORT_A,0);
	ecrobot_set_motor_speed(NXT_PORT_B,0);
	ecrobot_set_motor_speed(NXT_PORT_C,0);
	*/
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
		cmd_turn = (-1)*turn_speed;
	} else {
		cmd_turn = turn_speed;
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
int shock(int target){

	int result=0;

	//電圧降下の最小値を更新
	if(min_vol>ecrobot_get_battery_voltage())
		min_vol=ecrobot_get_battery_voltage();

	//ジャイロ及び電圧降下から衝撃検知
	if(target <= battery_value-min_vol)
	{
		result = 1;
	}
	else
		result = 0;

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

	static const float t_Kp = 2.85;

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
	static int step_count = 0;

	switch (setting_mode){
		case (TYREAL) :
			do_tyreal(&Kp,&Ki,&Kd);
			if(ecrobot_get_touch_sensor(NXT_PORT_S4) == TRUE)
			{
				systick_wait_ms(500);
				setting_mode = RN_SETTINGMODE_START;
			}
			break;

			//走行開始前
		case (RN_SETTINGMODE_START):
			RN_calibrate();		//キャリブレーション
			
			ecrobot_set_motor_speed(NXT_PORT_A,0);
			ecrobot_set_motor_speed(NXT_PORT_B,0);
			ecrobot_set_motor_speed(NXT_PORT_C,0);
			break;

			//通常走行
		case (RN_RUN):
			wait_count++;
			RA_linetrace_PID(30);

			if(RN_rapid_speed_up_signal_recevie() == 1)
			{
				setting_mode = RN_STEP_RAPID;
				revL = nxt_motor_get_count(NXT_PORT_C);
				revR = nxt_motor_get_count(NXT_PORT_B);

				distance_before_step = fabs(CIRCUMFERENCE/360.0 * ((revL+revR)/2.0));	//段差突入時の距離を測定
			}

			if(ecrobot_get_touch_sensor(NXT_PORT_S4) == TRUE)
			{
				ecrobot_sound_tone(932, 512, VOL);
				systick_wait_ms(100);

				setting_mode = TYREAL;
			}

			//直角カーブ部分
			
			if(ecrobot_get_light_sensor(NXT_PORT_S3) < RIGHT_ANGLE_LIGHT_VALUE && wait_count > 300)
			{
				ecrobot_sound_tone(880, 512, 30);
				setting_mode = RN_STEP_TURN_LEFT;
				wait_count = 0;
			}
			
			break;

			//加速
		case (RN_STEP_RAPID):
			RA_linetrace_PID(25);
			gyro_offset += 17;
			wait_count = 0;
			setting_mode = RN_STEP_SHOCK;
			break;

			//段差検知
		case (RN_STEP_SHOCK):
			RA_linetrace_PID(25);
			wait_count++;

			if(wait_count > 100)
			{
				if(shock(STEP_BATTERY) == 1)
				{
					min_vol = battery_value;
					setting_mode = RN_STEP_SLOW;
				}
			}
			
			revL = nxt_motor_get_count(NXT_PORT_C);
			revR = nxt_motor_get_count(NXT_PORT_B);

			distance_gyro_up = fabs(CIRCUMFERENCE/360.0 * ((revL+revR)/2.0));	//段差突入時の距離を測定

			break;

			//減速
		case (RN_STEP_SLOW):
			RA_linetrace_PID(25);
			gyro_offset -= 34;
			ecrobot_sound_tone(880, 512, 30);
			setting_mode = RN_STEP_STAY;
			wait_count = 0;
			break;

			//留まる
		case (RN_STEP_STAY):
			RA_linetrace_PID(25);
			wait_count++;

			if(wait_count == 85)
				gyro_offset += 16;
			
			if(wait_count >= 300)
			{
				setting_mode = RN_STEP_SECOND;

				wait_count = 0;
			}
			
			break;

			//二段目
		case (RN_STEP_SECOND):
			if(step_count == 0)
			//RA_linetrace_PID(25);
			RA_linetrace_PID(0);

			else if(step_count == 1)
			{
				RA_linetrace_PID(0);
				cmd_turn = RA_wheels(cmd_turn);
			}

			if(RN_rapid_speed_up_signal_recevie() == 1)
			{
				step_count = 1;
				setting_mode = RN_STEP_RAPID;
			}
			break;

			//直角カーブ
		case (RN_STEP_TURN_LEFT):
			cmd_forward = 0;
			cmd_turn = 0;
			if(wait_count == 0)
			{
				ecrobot_set_motor_rev(NXT_PORT_B, 0);
				ecrobot_set_motor_rev(NXT_PORT_C, 0);
			}

			wait_count++;

			if(ecrobot_get_motor_rev(NXT_PORT_B) <= RIGHT_ANGLE_AIM)
			{
				/* 回転する */
				ecrobot_set_motor_speed(NXT_PORT_B, 50);
				ecrobot_set_motor_speed(NXT_PORT_C, -50);
			}
			else
			{
				/* 止まる */
				ecrobot_set_motor_speed(NXT_PORT_B, 0);
				ecrobot_set_motor_speed(NXT_PORT_C, 0);
				wait_count = 0;
				revL = nxt_motor_get_count(NXT_PORT_C);
				revR = nxt_motor_get_count(NXT_PORT_B);
				distance_turn_clear = fabs(CIRCUMFERENCE/360.0 * ((revL+revR)/2.0));	//段差突入時の距離を測定
				setting_mode = RN_STEP_TURN_FORWARD;
			}
	
			break;

			//カーブ後直進
		case (RN_STEP_TURN_FORWARD):
			RA_linetrace(20,10);
			revL = nxt_motor_get_count(NXT_PORT_C);
			revR = nxt_motor_get_count(NXT_PORT_B);
			distance_turn_after = fabs(CIRCUMFERENCE/360.0 * ((revL+revR)/2.0));	//段差突入時の距離を測定
			if(distance_turn_after - distance_turn_clear > 5)
			{
				setting_mode = RN_STEP_TURN_TAILUP;
				ecrobot_sound_tone(880, 512, 30);
			}
			break;

			//尻尾→倒立
		case (RN_STEP_TURN_TAILUP):
			if(wait_count < 200)
			{
				nxt_motor_set_speed(NXT_PORT_C, 0, 1);
				nxt_motor_set_speed(NXT_PORT_B, 0, 1);
			}
			wait_count++;

			//尻尾持ち上げ補助
			if(wait_count == 200)
			{
				tail_mode = RN_TAILDOWN;
				
				ecrobot_set_motor_speed(NXT_PORT_B, -15);	//モータに速度を送る
				ecrobot_set_motor_speed(NXT_PORT_C, -15);	//モータに速度を送る
			}
			
			if(wait_count == 400)
			{
				ecrobot_set_motor_speed(NXT_PORT_B, 0);	//モータに速度を送る
				ecrobot_set_motor_speed(NXT_PORT_C, 0);	//モータに速度を送る	
			}
			
			/*
			if(t_angle == ANGLEOFDOWN && wait_count >= 1000)
			{
				tail_mode_change(1,ANGLEOFPUSH,0,10);
				nxt_motor_set_speed(NXT_PORT_C, 0, 1);
				nxt_motor_set_speed(NXT_PORT_B, 0, 1);
			}
			*/
			/*
			if(t_angle == ANGLEOFDOWN && wait_count >= 1050)
			{		

				//tail_mode_change(1,ANGLEOFUP,0,2);

				ecrobot_set_motor_rev(NXT_PORT_B,0);
				ecrobot_set_motor_rev(NXT_PORT_C,0);
				ecrobot_set_motor_speed(NXT_PORT_B,0);
				ecrobot_set_motor_speed(NXT_PORT_C,0);
				
				runner_mode_change(1);
				RA_hensareset();
				balance_init();
				wait_count=0;
				cmd_forward=0;
				
			}
			*/
			break;

				//強制停止
		case(RN_STOP):
			cmd_forward = 0;
			cmd_turn = 0;
			//nxt_motor_set_speed(NXT_PORT_C, 0, 1);
			//nxt_motor_set_speed(NXT_PORT_B, 0, 1);
			break;

		default:
			break;
	}
}

//急加速用関数
int rapid_speed_up(int target_gyro){

	static int rapid_speed_up_counter = 0;

	if(rapid_speed_up_counter >= 0)
	{
		if(GYRO_OFFSET > (target_gyro + GYRO_OFFSET))
			gyro_offset--;
		else
			gyro_offset++;

		rapid_speed_up_counter = 0;
	}

	rapid_speed_up_counter++;

	if((target_gyro + GYRO_OFFSET) == gyro_offset)
		return 1;
	else
		return 0;

}

static int RN_rapid_speed_up_signal_recevie(void)
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
		if (rx_buf[0] == rapid_SPEED_UP_SIGNAL)
		{
			start = 1;
			stepflag = 1;
		}
		
		ecrobot_send_bt(rx_buf, 0, rx_len); /* 受信データをエコーバック */
	}

	else if(ecrobot_get_touch_sensor(NXT_PORT_S4) == TRUE)
		start = 1; 

	return start;
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
						runner_mode = RN_MODE_BALANCEOFF;
						tail_mode = RN_TAILDOWN;
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
			nxt_motor_set_speed(NXT_PORT_C, cmd_forward + cmd_turn/2, 1);
			nxt_motor_set_speed(NXT_PORT_B, cmd_forward - cmd_turn/2, 1);
			break;

		default:
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
	//ecrobot_status_monitor(target_subsystem_name);	//モニタ出力
	TerminateTask();
}

//ログ送信管理(50ms)
TASK(LogTask)
{
	logSend(velocity,shock(STEP_BATTERY),distance_gyro_up - distance_before_step,battery_value - ecrobot_get_battery_voltage(),
			position_x,position_y,distance_turn_after - distance_turn_clear);		//ログ取り
	TerminateTask();
}

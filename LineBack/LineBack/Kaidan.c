

#include "Kaidan.h"
#include "logSend.h"
#include "math.h"
#include "tyreal_light_ver.h"
#include "LineBack.h"
#include "RobotPosutre.h"
#include "Factory.h"
#include "Calibration.h"
#include "PIDControl.h"
#include "TailControl.h"

/*
 *	各種変数定義
 */
/*
//ライントレース用目標値
static unsigned int BLACK_VALUE;	//黒値
static unsigned int WHITE_VALUE;	//白値
static unsigned int GRAY_VALUE;		//灰色値（現在は黒と白の平均値）
*/

//ジャイロオフセット値
//static unsigned int GYRO_OFFSET;	

static int counter = 0;


//尻尾設定角度
#define ANGLEOFDOWN 95		//降下目標角度
#define ANGLEOFUP 0					//上昇目標角度

//速度調節係数
#define SPEED_COUNT 20

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


static double min_vol;
static int stepflag = 0;

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


typedef enum{
	RN_SETTINGMODE_START,		//初期状態
	RN_RUN,						//基本走行（ライントレース）
	RN_STOP,					//停止
	RN_STEP_RAPID,
	RN_STEP_SHOCK,
	RN_STEP_SLOW,
	RN_STEP_STAY,
	RN_STEP_SECOND,
	TYREAL,
	RN_LINEBACK
} RN_SETTINGMODE;


//尻尾の状態
typedef enum{
	RN_TAILDOWN,				//尻尾降下
	RN_TAILUP,					//尻尾上昇
} RN_TAILMODE;



RN_SETTINGMODE setting_mode = RN_SETTINGMODE_START;
//RN_SETTINGMODE setting_mode = TYREAL;
RN_TAILMODE tail_mode = RN_TAILDOWN;


//段差検知関連マクロ、プロトタイプ

/*	
 *	各種関数定義
 */

//各種プライベート関数

//void RA_linetrace_PID(int forward_speed);
//int RA_speed(int forward_speed);

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
	factory();

	/*
	ecrobot_set_motor_speed(NXT_PORT_A,0);
	ecrobot_set_motor_speed(NXT_PORT_B,0);
	ecrobot_set_motor_speed(NXT_PORT_C,0);
	*/
}


//後始末処理
void ecrobot_device_terminate(void)
{
	tail_mode = RN_TAILDOWN;
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

/*
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
*/

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

//電圧値チェック関数（使わない）
void battery_average_check(void)
{
	if(bt_counter == 40)
	{
		battery_average = now_battery/40;

		if(battery_average > before_battery)
			average_flag = 1;
		else
			average_flag = 0;

		before_battery = battery_average;
		
		now_battery = 0;
		bt_counter = 0;
	}
	else
	{
		now_battery += ecrobot_get_gyro_sensor(NXT_PORT_S1);
		bt_counter++;
	}
}

//走行設定関数
void RN_setting()
{
	static int step_count = 0;
	static int counter=0;
	counter++;

	//前進量と旋回量
	ControlVals controlVals;
	controlVals.forward_val=0;
	controlVals.turn_val=0;

	switch (setting_mode){
		case (TYREAL) :
			//do_tyreal(&Kp,&Ki,&Kd);
			if(ecrobot_get_touch_sensor(NXT_PORT_S4) == TRUE)
			{
				systick_wait_ms(500);
				setting_mode = RN_SETTINGMODE_START;
			}
			break;

			//走行開始前
		case (RN_SETTINGMODE_START):
		if(Calibration_doCalibrate(&mCalibration)==1){
			//RobotPosture_setPostureMode(&mRobotPosture,TAIL_RUNNING);
			RobotPosture_setPostureMode(&mRobotPosture,CONS_TURN_BALANCING);
			//RobotPosture_setPostureMode(&mRobotPosture,BALANCING);
			tail_mode = RN_TAILUP;
			setting_mode = RN_RUN;
			

		}
			//		RN_calibrate();		//キャリブレーション
			/*
			ecrobot_set_motor_speed(NXT_PORT_A,0);
			ecrobot_set_motor_speed(NXT_PORT_B,0);
			ecrobot_set_motor_speed(NXT_PORT_C,0);*/
			break;

	
			//通常走行
		case (RN_RUN):
			controlVals.forward_val=0/*RA_speed(25)*/;
//			controlVals.turn_val=RA_linetrace_PID(controlVals.forward_val);

			controlVals.turn_val=0;//PIDControl_PIDLineTrace(&mPIDControl,controlVals.forward_val);

			RobotPosture_robotPostureControl(&mRobotPosture,controlVals);
			//test code
			setting_mode= RN_LINEBACK;
			//test code edd
		//if(GYRO_OFFSET - 100 > ecrobot_get_gyro_sensor(NXT_PORT_S1) || GYRO_OFFSET + 100 < ecrobot_get_gyro_sensor(NXT_PORT_S1))
				if(LineBack_debugLineBackSignalReceive(&mLineBack) == 1)
			{	
				ecrobot_sound_tone(932, 512, VOL);
				setting_mode= RN_LINEBACK;
			}
		//	cmd_turn = RA_wheels(cmd_turn);
			
			/*
			if(RN_rapid_speed_up_signal_recevie() == 1)
			{
				setting_mode = RN_STEP_RAPID;
				revL = nxt_motor_get_count(NXT_PORT_C);
				revR = nxt_motor_get_count(NXT_PORT_B);

				distance_before_step = fabs(CIRCUMFERENCE/360.0 * ((revL+revR)/2.0));	//段差突入時の距離を測定
			}
			*/
			
			break;
		
		case (RN_LINEBACK):
			if(LineBack_doLineBack(&mLineBack)==1){ //ライン復帰に成功したら１が返ってくる
			setting_mode = RN_RUN;
			}
	
			/*
			//加速
		case (RN_STEP_RAPID):
			RA_linetrace_PID(25);
			//if(rapid_speed_up(17) == 1)
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
			//if(rapid_speed_up(-34) == 1)
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


*/
			//強制停止
		case(RN_STOP):
			cmd_forward=0;
			cmd_turn=0;
			if(ecrobot_get_touch_sensor(NXT_PORT_S4) == TRUE)
			{setting_mode= RN_RUN;

			}
			/*
			nxt_motor_set_speed(NXT_PORT_C, 0, 1);
			nxt_motor_set_speed(NXT_PORT_B, 0, 1);
			*/
			break;

		default:
			break;
	}
}

//急加速用関数
int rapid_speed_up(int target_gyro){
	/*
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
		*/
	return 0;
}

 int RN_rapid_speed_up_signal_recevie(void)
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
 int remote_start(void)
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
	//RN_modesetting();	//走行体状態
	//tailcontrol();	
	RN_setting();
	TailControl_PIDTailControl(&mTailControl);
			//尻尾コントロール
	self_location();	//自己位置同定

	TerminateTask();
}

//走行状態管理(5ms)
TASK(ActionTask2)
{		
	battery_average_check();
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
	logSend(0,shock(STEP_BATTERY),theta*(180/M_PI)/*distance_gyro_up - distance_before_step*/,ecrobot_get_gyro_sensor(NXT_PORT_S1),
			position_x,position_y/*GyroVariation_getGyroSensorVariation(&mGyroVariation)*/,position_y);		//ログ取り
	TerminateTask();
}

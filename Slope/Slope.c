/*
 * Slope.c
 * 坂道プログラム
 */


#include "Slope.h"
#include "Factory.h"


/*
 *	各種定義
*/


//尻尾設定角度
#define ANGLEOFSTOP 105				//直立停止状態角度
#define ANGLEOFDOWN 95 			//降下目標角度
#define ANGLEOFUP 0					//上昇目標角度

#define PI 3.141592

//速度カウンタの上限値
#define SPEED_COUNT 10

//ジャイロ振幅値
#define PM_GYRO 65

//ジャイロオフセット値
static unsigned int GYRO_OFFSET;

//車輪の円周[cm]
#define CIRCUMFERENCE 25.8			//車輪の円周

/* 車輪半径、走行体幅*/
#define WHEEL_R		41	//[mm]
#define MACHINE_W	162	//[mm]

/*
 *	グローバル変数
 */

//ライントレース用目標値
static unsigned int BLACK_VALUE;	//黒値
static unsigned int WHITE_VALUE;	//白値
static unsigned int GRAY_VALUE;		//灰色値（現在は黒と白の平均値）

//速度調節カウンタ（カウンタが上限に達する毎に速度が1上昇
static int speed_counter = 0;


//PID制御用偏差値
static float hensa;					//P制御用
static float i_hensa = 0;			//I制御用
static float d_hensa = 0;			//D制御用
static float bf_hensa = 0;


//ライントレース時PID制御用係数
static float Kp = 1.85;				//P制御用
static float Ki = 2.6;				//I制御用
static float Kd = 0.003;			//D制御用
/*
static float Kp = 0.648;
static float Ki = 1.8;
static float Kd = 0.0405;
	*/
//尻尾PI制御用係数
static float t_Kp = 1.85;			//P制御用
static float t_Ki = 0;				//I制御用

//尻尾制御用変数
static int t_angle = 0;				//現在の角度
static int t_count = 0;				//制御速度調節用カウンタ
static int t_value = 0;				//角度目標値
static int t_count_limit = 0;		//カウンタ最大値
static int t_up = 0;				//増減値

//全体用カウンタ（時間稼ぎ用）
static int wait_count = 0;

//超音波センサ目標値
static int target_sonar = 20;

//超音波センサ用フラグ
static int sonarflag;

static int sonarvalue;

//ジャイロセンサオフセット計算用変数
static U32	gyro_offset = 0;    /* gyro sensor offset value */

/* 自己位置同定用変数 */
float d_theta_r;			//1ステップ当たりの右車輪回転角度
float d_theta_l;			//1ステップ当たりの左車輪回転角度

static float d_theta_r_t = 0;		//前回のステップの右車輪回転角度
static float d_theta_l_t = 0;		//前回のステップの左車輪回転角度

static double omega_r;			//右車輪の回転角速度
static double omega_l;			//左車輪の回転角速度

float v_r;				//右車輪の回転速度
float v_l;				//左車輪の回転速度

float v;					//走行体の走行速度
float omega;				//走行体の回転角速度

static float x_r = 0;		//車体のX座標
static float y_r = 0;		//車体のY座標
static float theta_R = 0;	//車体の角度

static float x_r_zero = 0;	//X座標初期値
static float y_r_zero = 0;	//Y座標初期値
static float theta_R_zero = 0;	//車体角度初期値

/* バランスコントロールへ渡すコマンド用変数 */
S8  cmd_forward, cmd_turn;
/* バランスコントロールから返されるモータ制御用変数 */
S8	pwm_l, pwm_r;


//距離計測用変数
int revL = 0;
int revR = 0;

int distance_before_slope;	//ルックアップゲート通過前距離
int distance_now_slope;	//ルックアップゲート通過中距離
int distance_peak_slope;
int distance_after_slope;


static double min_vol;
static int stepflag = 0;

//バッテリ電圧値状態
static U32	battery_value;

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
	RN_SLOPE_START,
	RN_SLOPE_DOWN,
	RN_SLOPE_AFTER,
	RN_SLOPE_END,
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
 *	各種プライベート関数定義
 */


//各種プライベート関数
void RN_calibrate();
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
void sonarcheck(void);
void runner_mode_change(int flag);
int getsonarflag(int target_sonar);
void tailpower(float value);
void tail_mode_change(int mode,int value,int limit,int t_up);
void RA_linetrace_PID_balanceoff(int forward_speed);
void getsonarvalue(void);

void RA_hensareset(void);


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
const char target_subsystem_name[] = "Slope";



/*
 *	関数実装
 */


//初期処理関数（プログラムの最初に呼び出し）
void ecrobot_device_initialize(void)
{
	factory();										//全オブジェクト初期化
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

	nxt_motor_set_speed(NXT_PORT_C, forward_speed - cmd_turn/2, 1);
	nxt_motor_set_speed(NXT_PORT_B, forward_speed + cmd_turn/2, 1);
}


//PID制御ライントレース関数
void RA_linetrace_PID(int forward_speed) {

	RA_speed(forward_speed,1);	//速度を段階的に変化

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

//PID制御ライントレース関数（バランサーOFF用）
void RA_linetrace_PID_balanceoff(int forward_speed){
	RA_speed(forward_speed,1);		//速度を段階的に変化

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

	nxt_motor_set_speed(NXT_PORT_C, forward_speed + cmd_turn/2, 1);
	nxt_motor_set_speed(NXT_PORT_B, forward_speed - cmd_turn/2, 1);
}

//段階的加速用関数
void RA_speed(int limit,int s_Kp){

	static int forward_speed;

	speed_counter++;						//速度調節用カウンタ

	if(speed_counter >= SPEED_COUNT)		//カウンタが最大で速度変更へ
	{

		forward_speed = cmd_forward;

		if(limit-forward_speed >= 0){
			forward_speed += s_Kp;		//指定量だけ速度上昇

			if(forward_speed > limit)
				forward_speed = limit;
		}
		else{
			forward_speed -= s_Kp;		//指定量だけ速度減少

			if(forward_speed < limit)
				forward_speed = limit;
		}

		cmd_forward = forward_speed;
		speed_counter = 0;					//カウンタリセット
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

	if (GRAY_VALUE > light_value)		//輝度値が目標値より大きいか判断
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
void sonarcheck(void)
{
	if(ecrobot_get_sonar_sensor(NXT_PORT_S2) <= target_sonar)	//超音波センサの値が目標値以下か判断しフラグ変更
	{
		sonarflag = 1;
	}
	else
		sonarflag = 0;
}

//超音波センサ状態確認関数
int getsonarflag(int target)
{
	target_sonar = target;				//超音波センサの目標値を指定

	return sonarflag;					//指定した値に近づいているかどうかを返す（1:範囲内）
}

void getsonarvalue(void)
{
	sonarvalue = ecrobot_get_sonar_sensor(NXT_PORT_S2);
}

//走行体モード変更関数（主にバランサーのON/OFF）
void runner_mode_change(int flag)
{
	switch(flag){
	case 0:
		runner_mode = RN_MODE_INIT;			//走行体初期状態
		break;
	case 1:
		runner_mode = RN_MODE_BALANCE;		//バランサーON
		break;
	case 2:
		runner_mode = RN_MODE_BALANCEOFF;	//バランサーOFF
		break;
	default:
		break;
	}
}

//尻尾制御パラメータ変更関数
void tailpower(float value)
{
	t_Kp = value;							//引数の値に尻尾制御のパラメータを変更
}


//尻尾角度調節関数
void taildown(){

	//X-ecrobot_get_motor_rev(NXT_PORT_S4) のX = 目標値
	//目標値を1ずつ調節(現行目標値t_angleそのものを最終目標値t_valueへ近づけていく)
	//t_angleの初期値は現在の角度

	static float t_hensa = 0;	//尻尾角度の目標値との差
	static float t_ihensa = 0;	//I制御用偏差

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
				t_angle = t_value;

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
				t_angle = t_value;

			break;

		default:
			break;

	}

	t_ihensa = t_ihensa+(t_hensa*0.0004);		//I制御用偏差

	t_speed = (t_Kp*t_hensa + t_Ki*t_ihensa);	//偏差を元に速度調節

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

//bluetoothログ送信関数
void logSend(S8 data1, S8 data2, S16 adc1, S16 adc2, S16 adc3, S16 adc4){
            U8 data_log_buffer[32];

            *((U32 *)(&data_log_buffer[0]))  = (U32)systick_get_ms();
            *(( S8 *)(&data_log_buffer[4]))  =  (S8)data1;
            *(( S8 *)(&data_log_buffer[5]))  =  (S8)data2;
            *((U16 *)(&data_log_buffer[6]))  = (U16)ecrobot_get_light_sensor(NXT_PORT_S3);
            *((S32 *)(&data_log_buffer[8]))  = (S32)nxt_motor_get_count(0);
            *((S32 *)(&data_log_buffer[12])) = (S32)nxt_motor_get_count(1);
            *((S32 *)(&data_log_buffer[16])) = (S32)nxt_motor_get_count(2);
            *((S16 *)(&data_log_buffer[20])) = (S16)adc1;
            *((S16 *)(&data_log_buffer[22])) = (S16)adc2;
            *((S16 *)(&data_log_buffer[24])) = (S16)adc3;
            *((S16 *)(&data_log_buffer[26])) = (S16)adc4;
            *((S32 *)(&data_log_buffer[28])) = (S32)ecrobot_get_sonar_sensor(NXT_PORT_S2);

            ecrobot_send_bt_packet(data_log_buffer, 32);
}


//走行状態設定関数
void RN_setting()
{
	int forward_speed;
	PWMValues pValues;


	switch (setting_mode){

			//キャリブレーション状態
		case (RN_SETTINGMODE_START):
			if(Calibration_doCalibrate(&mCalibration) == 1)
				setting_mode = RN_RUN;
			break;

		case (RN_SPEEDZERO):
			//cmd_forward = 0;
			//cmd_turn = RA_wheels(cmd_turn);
			//wait_count++;
			//if(wait_count >= 200)
			//{
				setting_mode = RN_RUN;
				//wait_count = 0;
			//}
			break;

			//通常走行状態
		case (RN_RUN):
			//wait_count++;
			//RA_linetrace_PID_balanceoff(100);
			pValues = PWMValGenerator_calTailRunPWM(&mPWMValGenerator,50,PIDCalcDebug_PIDLinetrace(&mPIDCalcDebug));
			nxt_motor_set_speed(NXT_PORT_C, pValues.pwmL, 1);
			nxt_motor_set_speed(NXT_PORT_B, pValues.pwmR, 1);
			/*
			if(GYRO_OFFSET - 30 > (U32)ecrobot_get_gyro_sensor(NXT_PORT_S1) && wait_count > 500)
			{
				ecrobot_sound_tone(880, 512, 30);
				setting_mode = RN_SLOPE_START;
				revL = nxt_motor_get_count(NXT_PORT_C);
				revR = nxt_motor_get_count(NXT_PORT_B);
				distance_before_slope = fabs(CIRCUMFERENCE/360.0 * ((revL+revR)/2.0));
				wait_count = 0;
			}
			*/
			break;

		case (RN_SLOPE_START):
			RA_linetrace_PID_balanceoff(70);

			revL = nxt_motor_get_count(NXT_PORT_C);
			revR = nxt_motor_get_count(NXT_PORT_B);
			distance_now_slope = fabs(CIRCUMFERENCE/360.0 * ((revL+revR)/2.0));

			if(distance_now_slope - distance_before_slope > 30)
			{
				setting_mode = RN_SLOPE_DOWN;
			}

			break;

		case (RN_SLOPE_DOWN):
			RA_linetrace_PID_balanceoff(55);

			if(GYRO_OFFSET - 30 > (U32)ecrobot_get_gyro_sensor(NXT_PORT_S1))
			{
				ecrobot_sound_tone(880, 512, 30);
				setting_mode = RN_SLOPE_AFTER;
				revL = nxt_motor_get_count(NXT_PORT_C);
				revR = nxt_motor_get_count(NXT_PORT_B);
				distance_peak_slope = fabs(CIRCUMFERENCE/360.0 * ((revL+revR)/2.0));
			}
			break;

		case (RN_SLOPE_AFTER):
			RA_linetrace_PID_balanceoff(55);

			revL = nxt_motor_get_count(NXT_PORT_C);
			revR = nxt_motor_get_count(NXT_PORT_B);
			distance_after_slope = fabs(CIRCUMFERENCE/360.0 * ((revL+revR)/2.0));

			if(distance_after_slope - distance_peak_slope > 30)
			{
				setting_mode = RN_SLOPE_END;
			}

			break;

		case (RN_SLOPE_END):
			RA_linetrace_PID_balanceoff(65);
			break;

		default:
			break;
	}
}



//自己位置同定関数
void self_location()
{
	d_theta_l = (float)nxt_motor_get_count(NXT_PORT_C) * PI / 180.0;
	d_theta_r = (float)nxt_motor_get_count(NXT_PORT_B) * PI / 180.0;

	omega_l = (d_theta_l - d_theta_l_t)/0.004;
	omega_r = (d_theta_r - d_theta_r_t)/0.004;

	v_l = (WHEEL_R * 0.1) * omega_l;
	v_r = (WHEEL_R * 0.1) * omega_r;

	v = (v_r + v_l) / 2.0;
	omega = (v_r - v_l) / (MACHINE_W * 0.1);

	d_theta_l_t = d_theta_l;
	d_theta_r_t = d_theta_r;

	theta_R += omega * 0.004 + theta_R_zero;
	x_r += v * cos(theta_R) * 0.004 + x_r_zero;
	y_r += v * sin(theta_R) * 0.004 + y_r_zero;

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
	logSend(v,cmd_turn,distance_now_slope - distance_before_slope,ecrobot_get_gyro_sensor(NXT_PORT_S1),		//Bluetoothを用いてデータ送信
			BLACK_VALUE,WHITE_VALUE);

	sonarcheck();											//超音波センサ状態管理
	getsonarvalue();

	TerminateTask();
}



/******************************** END OF FILE ********************************/

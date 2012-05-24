/*
 * logtrace.c
 * ライントレース及び段差検知プログラム
 */


#include "logtrace.h"


//ライントレース用目標値
static unsigned int BLACK_VALUE;	//黒値
static unsigned int WHITE_VALUE;	//白値
static unsigned int GRAY_VALUE;		//灰色値（現在は黒と白の平均値）

static int counter = 0;

//尻尾設定角度
#define ANGLEOFDOWN 0				//降下目標角度
#define ANGLEOFUP 0					//上昇目標角度
#define ANGLEOFPUSH 210				//押上目標角度（未使用）

//速度調節係数
//#define SPEED_COUNT 10
#define SPEED_COUNT 10

//バッテリ降下値
#define DOWN_BATTERY 550			//バッテリ降下値


//ジャイロ振幅値
#define PM_GYRO 65


//車輪の円周[cm]
#define CIRCUMFERENCE 25.8			//車輪の円周


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

static double min_vol;

//ジャイロセンサオフセット計算用変数
static U32	gyro_offset = 0;    /* gyro sensor offset value */
static U32	avg_cnt = 0;		/* average count to calc gyro offset */
static U32	cal_start_time;		/* calibration start time */


//バッテリ電圧値状態
static U32	battery_value;


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


//システム全体の状態
typedef enum{
	RN_MODE_INIT, 					//初期状態
	RN_MODE_CONTROL,				//倒立制御ON
	RN_MODE_STOP,					//倒立制御OFF
	RN_MODE_CONTROL_2				//倒立制御ON_2
} RN_MODE;


//キャリブレーションの状態
typedef enum{
	RN_SETTINGMODE_START,		//初期状態
	RN_RUN,						//基本走行（ライントレース）
	RN_SLOW_RUN_WAIT,			//基本走行停止
	RN_SLOW_RUN,				//低速走行
	RN_BACK_RUN,				//反対走行
	RN_HIGH_RUN,				//高速走行
	RN_STOP,						//停止
	RN_STOP_WAIT
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
RN_TAILMODE tail_mode = RN_TAILUP;


//各種プライベート関数
void RN_calibrate();
void RN_setting();
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


//カウンタの宣言
DeclareCounter(SysTimerCnt);


//タスクの宣言
DeclareTask(ActionTask);
DeclareTask(ActionTask2);
DeclareTask(DisplayTask);
DeclareTask(LogTask);


//液晶ディスプレイに表示するシステム名設定
const char target_subsystem_name[] = "Logtrace";


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

	hensa = (float)GRAY_VALUE - (float)ecrobot_get_light_sensor(NXT_PORT_S3);

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

	RA_speed(forward_speed,1);

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

	if(min_vol>ecrobot_get_battery_voltage())
		min_vol=ecrobot_get_battery_voltage();

	if((ecrobot_get_gyro_sensor(NXT_PORT_S1) <= gyro_offset-PM_GYRO ||
		ecrobot_get_gyro_sensor(NXT_PORT_S1) >= gyro_offset+PM_GYRO) &&
			min_vol <= battery_value-DOWN_BATTERY)
	{
		ecrobot_sound_tone(880,512,30);
		tail_mode = RN_TAILDOWN;
		if(runner_mode == RN_MODE_CONTROL){
			revL = nxt_motor_get_count(NXT_PORT_C);
			revR = nxt_motor_get_count(NXT_PORT_B);
			distance_before = fabs(CIRCUMFERENCE/360.0 * ((revL+revR)/2.0));
			setting_mode = RN_SLOW_RUN;
			runner_mode = RN_MODE_CONTROL_2;
			min_vol = battery_value;
		}
	}
}


//ON-OFF制御用ライン判定
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


//bluetoothによるログ送信
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


//走行状態設定
void RN_setting()
{
	switch (setting_mode){
		case (RN_SETTINGMODE_START):
			RN_calibrate();
			break;

		case (RN_RUN):
			RA_linetrace_PID(25);
			revL = nxt_motor_get_count(NXT_PORT_C);
			revR = nxt_motor_get_count(NXT_PORT_B);
			distance_before = fabs(CIRCUMFERENCE/360.0 * ((revL+revR)/2.0));
			break;

		case(RN_SLOW_RUN):
			revL = nxt_motor_get_count(NXT_PORT_C);
			revR = nxt_motor_get_count(NXT_PORT_B);
			distance_slow = fabs(CIRCUMFERENCE/360.0 * ((revL+revR)/2.0));

			if((distance_before - distance_slow <= -7))
			{
				//gyro_offset -= 250;
				setting_mode = RN_STOP_WAIT;
			}
			else
			{
				RA_linetrace(0,1);
				cmd_turn = RA_wheels(cmd_turn);
			}

			//RA_speed(0,1);
			//cmd_turn = RA_wheels(cmd_turn);
			break;

		case(RN_BACK_RUN):
			RA_linetrace_PID(-15);
			break;
		case(RN_HIGH_RUN):
			//RA_linetrace_PID(20,0);
			//cmd_turn = RA_wheels(cmd_turn);
			break;
		case(RN_STOP_WAIT):
			wait_count += 1;
			//if(cmd_forward == 0)
			//{
				//systick_wait_ms(100);
				//runner_mode = RN_STOP;
				//setting_mode = RN_STOP;
			//}
			//else
			//{
				RA_linetrace(0,1);
				cmd_turn=RA_wheels(cmd_turn);
				if(cmd_forward == 0 && wait_count == 1000)
				{
					RA_hensareset();
					wait_count = 0;
					setting_mode = RN_RUN;
					runner_mode = RN_MODE_CONTROL;
					tail_mode = RN_TAILUP;
				}
			//}
			break;
		case(RN_STOP):
			nxt_motor_set_speed(NXT_PORT_C, 0, 1);
			nxt_motor_set_speed(NXT_PORT_B, 0, 1);
			break;
		default:
			break;
	}
}


//キャリブレーション
void RN_calibrate()
{

	//黒値
	while(1){
		if(ecrobot_get_touch_sensor(NXT_PORT_S4) == TRUE){
			ecrobot_sound_tone(880, 512, 30);
			BLACK_VALUE=ecrobot_get_light_sensor(NXT_PORT_S3);
			systick_wait_ms(500);
			break;
		}
	}

	//白値
	while(1){
		if(ecrobot_get_touch_sensor(NXT_PORT_S4) == TRUE){
			ecrobot_sound_tone(906, 512, 30);
			WHITE_VALUE=ecrobot_get_light_sensor(NXT_PORT_S3);
			systick_wait_ms(500);
			break;
		}
	}

	//灰色値計算
	GRAY_VALUE=(BLACK_VALUE+WHITE_VALUE)/2;

	//ジャイロオフセット及びバッテリ電圧値
	while(1){
		if(ecrobot_get_touch_sensor(NXT_PORT_S4) == TRUE){
			ecrobot_sound_tone(932, 512, 30);
			gyro_offset += (U32)ecrobot_get_gyro_sensor(NXT_PORT_S1);
			battery_value = ecrobot_get_battery_voltage();
			min_vol = battery_value;
			systick_wait_ms(500);
			break;
		}
	}

	while(1){
		if (ecrobot_get_touch_sensor(NXT_PORT_S4) == TRUE){
				ecrobot_sound_tone(982,512,30);
			break;
		}
	}

	//キャリブレーション終了
	while(1){
		if (ecrobot_get_touch_sensor(NXT_PORT_S4) != TRUE) {
			//ecrobot_sound_tone(950, 512, 30);
			setting_mode = RN_RUN;
			runner_mode = RN_MODE_CONTROL;
			tail_mode = RN_TAILUP;
			break;
		}
	}
}


//走行方法設定
void RN_modesetting()
{
	switch (runner_mode){
		case (RN_MODE_INIT):
			cmd_forward = 0;
			cmd_turn = 0;
			break;

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

		case (RN_MODE_CONTROL_2):
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
	RN_setting();
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
	logSend(min_vol,cmd_turn,ecrobot_get_battery_voltage(),ecrobot_get_gyro_sensor(NXT_PORT_S1),
			distance_before - distance_slow,counter);
	TerminateTask();
}


/******************************** END OF FILE ********************************/


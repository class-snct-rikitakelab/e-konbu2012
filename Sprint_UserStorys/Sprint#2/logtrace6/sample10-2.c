/*
 * sample10.c
 *
 * バランスして立ち、ライントレースする
 * (c) COPYRIGHT 2010 Afrel., Inc.
 * All rights reserved
 */

#include "sample10.h"


//ライントレース用目標値
//static unsigned int LIGHT_THRESHOLD=580;
static unsigned int BLACK_VALUE=600;
static unsigned int WHITE_VALUE=500;
static unsigned int GRAY_VALUE=550;


//尻尾設定角度
#define angleofdown 100
#define angleofup 0
#define angleofpush 210


//車輪の円周[cm]
#define circumference 25.8


//PID制御用偏差値
static float hensa;
static float i_hensa = 0;
static float d_hensa = 0;
static float bf_hensa = 0;


//PID用係数
static float Kp = 1.7;
static float Ki = 2.6;
static float Kd = 0.01;


//システム全体の状態
typedef enum{
	RN_MODE_INIT, 	
	RN_MODE_CONTROL,
	RN_MODE_STEP,
	RN_MODE_STOP,
	RN_MODE_GYRODOWN
} RN_MODE;


//キャリブレーションの状態
typedef enum{
	RN_SETTINGMODE_GYRO_START,
	RN_SETTINGMODE_END,
	RN_RUN,
	RN_SLOW_RUN_WAIT,
	RN_SLOW_RUN,
	RN_LOW_RUN,
	RN_HIGH_RUN,
	RN_STOP
} RN_SETTINGMODE;


//尻尾の状態
typedef enum{
	RN_TAILDOWN,
	RN_TAILUP,
	RN_TAILPUSH
} RN_TAILMODE;


//初期状態
RN_MODE runner_mode = RN_MODE_INIT;					/* NXTway-GS mode */
RN_SETTINGMODE setting_mode = RN_SETTINGMODE_GYRO_START;/* キャリブレーションの状態 */
RN_TAILMODE tail_mode = RN_TAILDOWN;


//ジャイロセンサオフセット計算用変数
static U32	gyro_offset = 0;    /* gyro sensor offset value */
static U32	avg_cnt = 0;		/* average count to calc gyro offset */
static U32	cal_start_time;		/* calibration start time */


//バッテリ電圧値状態
static U32	battery_value = 0;
//static int	motor_a = 0;


/* バランスコントロールへ渡すコマンド用変数 */
S8  cmd_forward, cmd_turn,forward_speed;
/* バランスコントロールから返されるモータ制御用変数 */
S8	pwm_l, pwm_r;
	
	
/*
 * キャリブレーション用のプライベート関数
 */
void RN_calibrate();
void RN_setting();
int online();
void RA_linetrace(int forward_speed, int turn_speed);
void RA_linetrace_P(int forward_speed,int resetflag);
void logSend(S8 data1, S8 data2, S16 adc1, S16 adc2, S16 adc3, S16 adc4);
void shock();
void taildown();
void startuptail();
void RA_linetrace_Ponly(int forward_speed);
void RA_speed(int forward_speed,int limit,float s_Kp);
int RA_wheels(int turn);

int revL = 0;
int revR = 0;
float distance_now = 0;
float distance_before = 0;

/*
 * ロボット制御用のプライベート関数
 */
int RN_move();

/*
 *カウンタの宣言
 */
DeclareCounter(SysTimerCnt);


/*
 *タスクの宣言
 */
DeclareTask(ActionTask);
DeclareTask(ActionTask2);
DeclareTask(DisplayTask);
DeclareTask(LogTask);


/*
 *液晶ディスプレイに表示するシステム名設定
 */
const char target_subsystem_name[] = "Logtrace";


/*
 *初期処理
 */
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
	ecrobot_set_motor_speed(NXT_PORT_A, 0);
	ecrobot_set_motor_speed(NXT_PORT_B, 0);
	ecrobot_set_motor_speed(NXT_PORT_C, 0);
	ecrobot_set_light_sensor_inactive(NXT_PORT_S3);
	ecrobot_term_sonar_sensor(NXT_PORT_S2);
	ecrobot_term_bt_connection();
}


/*--------------------------------------------------------------------------*/
/* OSEK hooks                                                               */
/*--------------------------------------------------------------------------*/
void StartupHook(void){}
void ShutdownHook(StatusType ercd){}
void PreTaskHook(void){}
void PostTaskHook(void){}
void ErrorHook(StatusType ercd){}


/*--------------------------------------------------------------------------*/
/* Function to be invoked from a category 2 interrupt                       */
/*--------------------------------------------------------------------------*/
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



void RA_linetrace(int forward_speed, int turn_speed) {

	taildown();

	cmd_forward = forward_speed;

	int light_value = 0;
	light_value = online();
	if (TRUE != light_value) {
		cmd_turn = turn_speed;
	} else {
		cmd_turn = (-1)*turn_speed;
	}

}

void RA_linetrace_P(int forward_speed,int resetflag) {

	if(resetflag==1)
	{
		hensa = 0;
		i_hensa = 0;
		d_hensa = 0;
		bf_hensa = 0;
	}

	shock();

	cmd_forward = forward_speed;

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

//	nxt_motor_set_speed(NXT_PORT_C, forward_speed + cmd_turn/2, 1);
//	nxt_motor_set_speed(NXT_PORT_B, forward_speed - cmd_turn/2, 1);

}

//比例して加速（P制御）
void RA_speed(int first_speed,int limit,float s_Kp){

	static float s_hensa;
	static int forward_speed;
	
	if(limit >=0)
		forward_speed += s_Kp;
	else
		forward_speed -= s_Kp;
	
	if(limit >=0){
		if(forward_speed >= limit)
			forward_speed = limit;
	}
	else{
		if(forward_speed <= limit)
			forward_speed = limit;
	}
	
	cmd_forward = forward_speed;
	
}


//2車輪の回転量の差をP制御で調節する（目標値に近づける）
int RA_wheels(int turn){
	float w_kp = 1.4;
	
	signed long def = ecrobot_get_motor_rev(NXT_PORT_B) - ecrobot_get_motor_rev(NXT_PORT_C);
	turn = w_kp * def;
	
	return turn;
}


void RA_linetrace_Ponly(int forward_speed){

	cmd_forward = forward_speed;

	hensa = (float)GRAY_VALUE - (float)ecrobot_get_light_sensor(NXT_PORT_S3);

	cmd_turn = -(1.4 * hensa);
	if (-100 > cmd_turn) {
		cmd_turn = -100;
	} else if (100 < cmd_turn) {
		cmd_turn = 100;
	}
}

void shock(void){

	static double min_vol;

	if(min_vol>ecrobot_get_battery_voltage()){
		min_vol=ecrobot_get_battery_voltage();
	}	

	if((ecrobot_get_gyro_sensor(NXT_PORT_S4) <= gyro_offset-65 || 
		ecrobot_get_gyro_sensor(NXT_PORT_S4) >= gyro_offset+65) &&
			min_vol <= battery_value-550){
		ecrobot_sound_tone(880,512,30);
		tail_mode = RN_TAILDOWN;
		if(runner_mode == RN_MODE_CONTROL){
		setting_mode = RN_SLOW_RUN;
		runner_mode = RN_MODE_GYRODOWN;
		}
	}
}


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

void taildown(){

	static const float t_Kp = 0.7;
	static const float t_b = 0;

	static float t_hensa = 0;
	static float t_speed = 0;

	switch(tail_mode){
		case(RN_TAILDOWN):
			t_hensa = angleofdown - ecrobot_get_motor_rev(NXT_PORT_A);
			break;

		case(RN_TAILUP):
			t_hensa = angleofup - ecrobot_get_motor_rev(NXT_PORT_A);
			break;

		case(RN_TAILPUSH):
			t_hensa = angleofpush - ecrobot_get_motor_rev(NXT_PORT_A);
			break;
		default:
			break;
	}

	t_speed = t_Kp*t_hensa + t_b;
		if (t_speed < -100) {
			t_speed = -100;
		}else if (t_speed > 100) {
			t_speed = 100;
		}
	ecrobot_set_motor_speed(NXT_PORT_A, t_speed);

}

void startuptail(){
	static const float ts_Kp = 0.7;
	static const float ts_b = 0;

	static float ts_hensa = 0;
	static float ts_speed = 0;

	ts_hensa = angleofdown - ecrobot_get_motor_rev(NXT_PORT_A);

	ts_speed = ts_Kp*ts_hensa + ts_b;
	if (ts_speed < -100) {
		ts_speed = -100;
	}else if (ts_speed > 100) {
		ts_speed = 100;
	}
	ecrobot_set_motor_speed(NXT_PORT_A, ts_speed);
}

void logSend(S8 data1, S8 data2, S16 adc1, S16 adc2, S16 adc3, S16 adc4)
        {
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

void RN_setting()
{
	switch (setting_mode){
		case (RN_SETTINGMODE_GYRO_START):
			RN_calibrate();
			break;

		case (RN_SETTINGMODE_END):
			setting_mode = RN_RUN;
			break;

		case (RN_RUN):
			RA_linetrace_P(15,0);
			revL = nxt_motor_get_count(NXT_PORT_C);
			revR = nxt_motor_get_count(NXT_PORT_B);
			distance_before = fabs(circumference/360.0 * ((revL+revR)/2.0));
			break;

		case (RN_SLOW_RUN_WAIT):
			RA_linetrace_P(0,0);
			systick_wait_ms(1000);
			gyro_offset += 150;
			runner_mode = RN_MODE_STOP;
			gyro_offset -= 150;
			nxt_motor_set_speed(NXT_PORT_C, 0, 1);
			nxt_motor_set_speed(NXT_PORT_B, 0, 1);
			systick_wait_ms(3000);
			setting_mode = RN_SLOW_RUN;
			//runner_mode = RN_MODE_STOP;
			break;

		case(RN_SLOW_RUN):
			RA_linetrace_P(-15, 0);
			//taildown();
			//cmd_forward = -15;
			//cmd_turn = 0;
			/*移動距離*/
			//RA_linetrace_P(-15,0);
			//RA_speed(15,-10,0.05);
			cmd_turn = RA_wheels(cmd_turn);
			revL = nxt_motor_get_count(NXT_PORT_C);
			revR = nxt_motor_get_count(NXT_PORT_B);
			distance_now = fabs(circumference/360.0 * ((revL+revR)/2.0));
			//nxt_motor_set_speed(NXT_PORT_C, -15+turn/2, 1);
			//nxt_motor_set_speed(NXT_PORT_B, -15+turn/2, 1);
			if(distance_before-distance_now >= 4){
				//runner_mode = RN_MODE_STOP;
				setting_mode = RN_LOW_RUN;
			}
			break;

		case(RN_LOW_RUN):
			//RA_linetrace_P(0,0);
			//nxt_motor_set_speed(NXT_PORT_C, 0, 1);
			//nxt_motor_set_speed(NXT_PORT_B, 0, 1);
			ecrobot_sound_tone(880,512,30);
			systick_wait_ms(1000);
			ecrobot_sound_tone(880,512,30);
			systick_wait_ms(1000);
			ecrobot_sound_tone(900,512,30);
			setting_mode = RN_HIGH_RUN;
			break;
		case(RN_HIGH_RUN):
			//runner_mode = RN_MODE_CONTROL;
			//nxt_motor_set_speed(NXT_PORT_C, 150, 1);
			//nxt_motor_set_speed(NXT_PORT_B, 150, 1);
		//RA_speed(0,150);
			systick_wait_ms(800);
			tail_mode = RN_TAILPUSH;
			systick_wait_ms(300);
			tail_mode = RN_TAILDOWN;
			systick_wait_ms(2000);
			setting_mode = RN_STOP;
			//runner_mode = RN_LOW_RUN;
			break;
		case(RN_STOP):
			nxt_motor_set_speed(NXT_PORT_C, 0, 1);
			nxt_motor_set_speed(NXT_PORT_B, 0, 1);
			break;
		default:
			break;
	}
}

void RN_calibrate()
{
	
	while(1){
		if(ecrobot_get_touch_sensor(NXT_PORT_S1) == TRUE){
			ecrobot_sound_tone(880, 512, 30);
			BLACK_VALUE=ecrobot_get_light_sensor(NXT_PORT_S3);
			systick_wait_ms(500);
			break;
		}
	}
	while(1){
		if(ecrobot_get_touch_sensor(NXT_PORT_S1) == TRUE){
			ecrobot_sound_tone(880, 512, 30);
			WHITE_VALUE=ecrobot_get_light_sensor(NXT_PORT_S3);
			systick_wait_ms(500);
			break;
		}
	}

	GRAY_VALUE=(BLACK_VALUE+WHITE_VALUE)/2;

	while(1){
		if(ecrobot_get_touch_sensor(NXT_PORT_S1) == TRUE){
			gyro_offset += (U32)ecrobot_get_gyro_sensor(NXT_PORT_S4);
			battery_value = ecrobot_get_battery_voltage();
			break;
		}
	}
	while(1){
		if (ecrobot_get_touch_sensor(NXT_PORT_S1) != TRUE) {
			ecrobot_sound_tone(900, 512, 30);
			setting_mode = RN_RUN;
			runner_mode = RN_MODE_CONTROL;
			tail_mode = RN_TAILUP;
			break;
		}
	}
}


TASK(LogTask)
{/*
	static S8 i,j;
	int sonar;

	sonar = ecrobot_get_sonar_sensor(NXT_PORT_S2);

	logSend(cmd_forward,cmd_turn,ecrobot_get_battery_voltage(),ecrobot_get_gyro_sensor(NXT_PORT_S4),
		distance_before-distance_now,ecrobot_get_motor_rev(NXT_PORT_A));

	ecrobot_status_monitor("Data Logging");
*/
	TerminateTask();
}


TASK(DisplayTask)
{
	/* モニタにシステム名を表示する */
	/*
	ecrobot_status_monitor(target_subsystem_name);
*/
	TerminateTask();
}


TASK(ActionTask)
{
	switch (runner_mode){
		case (RN_MODE_INIT):
			taildown();
			cmd_forward = 0;
			cmd_turn = 0;
			battery_value = ecrobot_get_battery_voltage();
			break;

		case (RN_MODE_CONTROL):
			balance_control(
				(F32)cmd_forward,
				(F32)cmd_turn,
				(F32)ecrobot_get_gyro_sensor(NXT_PORT_S4),
		 		(F32)gyro_offset,
				(F32)nxt_motor_get_count(NXT_PORT_C),
		 		(F32)nxt_motor_get_count(NXT_PORT_B),
				(F32)ecrobot_get_battery_voltage(),
				&pwm_l,
				&pwm_r);
			nxt_motor_set_speed(NXT_PORT_C, pwm_l, 1);
			nxt_motor_set_speed(NXT_PORT_B, pwm_r, 1);
			break;
		case (RN_MODE_GYRODOWN):
		
			balance_control(
				(F32)cmd_forward,
				(F32)cmd_turn,
				(F32)ecrobot_get_gyro_sensor(NXT_PORT_S4),
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
		case (RN_MODE_STEP):
			nxt_motor_set_speed(NXT_PORT_C, /*3*(cmd_forward + cmd_turn/2)*/120, 1);
			nxt_motor_set_speed(NXT_PORT_B, /*3*(cmd_forward - cmd_turn/2)*/120, 1);
			break;
		default:
			nxt_motor_set_speed(NXT_PORT_C, 0, 1);
			nxt_motor_set_speed(NXT_PORT_B, 0, 1);
			break;
	}
	TerminateTask();
}


TASK(ActionTask2)
{
	RN_setting();
	TerminateTask();
}


/******************************** END OF FILE ********************************/

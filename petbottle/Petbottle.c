/*
 * petbottle.c
 * ドリフトターン用ペットボトル検知プログラム
 */


#include "Petbottle.h"
#include "math.h"


/*
 *	各種定義
*/


//尻尾設定角度
#define ANGLEOFDOWN 95 				//降下目標角度
#define ANGLEOFUP 0					//上昇目標角度
#define ANGLEOFPUSH ANGLEOFDOWN+5				//押上目標角度（未使用）
#define ANGLEOFLOOKUP 56

#define PI 3.141592

//速度カウンタの上限値
#define SPEED_COUNT 10

//ジャイロ振幅値
#define PM_GYRO 65

//車輪の円周[cm]
#define CIRCUMFERENCE 25.8			//車輪の円周

#define CMD_START '1'    			//リモートスタートコマンド(変更禁止)

/* 車輪半径、走行体幅*/
#define WHEEL_R		41	//[mm]
#define MACHINE_W	162	//[mm]

#define BOTTLE 27	//[cm]
/*
 *	グローバル変数
 */

//ライントレース用目標値
static unsigned int BLACK_VALUE;	//黒値
static unsigned int WHITE_VALUE;	//白値
static unsigned int GRAY_VALUE;		//灰色値（現在は黒と白の平均値）
static unsigned int LOOKUP_BLACK_VALUE;		//角度がANGLEOFLOOKUP時の黒値
static unsigned int LOOKUP_WHITE_VALUE;		//角度がANGLEOFLOOKUP時の白値
static unsigned int LOOKUP_GRAY_VALUE;		//角度がANGLEOFLOOKUP地の灰色値（現在は黒と白の平均値）

static unsigned int LOOKUP_SONAR_VALUE;

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

static int get_sonar = 0;		//超音波センサの値
static int bottle_flag ;		//ペットボトルの判断
static int alarm_flag;

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


//状態
typedef enum{
	RN_SETTINGMODE_START,		//初期状態
	RN_RUN,				//基本走行（ライントレース）
	RN_DRIFTTURN,			//ドリフトターン
} RN_SETTINGMODE;

//尻尾の状態
typedef enum{
	RN_TAILDOWN,				//尻尾降下
	RN_TAILUP,					//尻尾上昇
} RN_TAILMODE;

//初期状態
RN_MODE runner_mode = RN_MODE_INIT;
RN_SETTINGMODE setting_mode = RN_SETTINGMODE_START;
RN_TAILMODE tail_mode = RN_TAILUP;

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
void sonar();
void get_bottle();
void alarm();

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
const char target_subsystem_name[] = "petbottle";



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
			{
				t_angle = t_value;
			}

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
			{
				t_angle = t_value;
			}

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
	switch (setting_mode){

			//キャリブレーション状態
		case (RN_SETTINGMODE_START):
			RN_calibrate();
			break;

			//通常走行状態
		case (RN_RUN):
			setting_mode = RN_DRIFTTURN;
			break;

		case (RN_DRIFTTURN):
			alarm();
			break;

		default:
			break;
	}
}

//キャリブレーション関数
void RN_calibrate()
{

	tail_mode_change(0,ANGLEOFDOWN,1,2);

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
			systick_wait_ms(500);
			break;
		}
	}

	//走行開始合図
	while(1)
	{
		//リモートスタート
		if(remote_start()==1)
		{
			ecrobot_sound_tone(982,512,30);
			tail_mode_change(1,ANGLEOFUP,0,2);
			setting_mode = RN_RUN;
			runner_mode = RN_MODE_BALANCE;
			break;
		}

		//タッチセンサスタート
		else if (ecrobot_get_touch_sensor(NXT_PORT_S4) == TRUE)
		{
			ecrobot_sound_tone(982,512,10);
			while(1)
			{
					if (ecrobot_get_touch_sensor(NXT_PORT_S4) != TRUE)
					{
						setting_mode = RN_RUN;
						runner_mode_change(2);
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

void sonar()
{
	
	get_sonar = ecrobot_get_sonar_sensor(NXT_PORT_S2);

}

void get_bottle()
{
	bottle_flag = 0;
	if(get_sonar < BOTTLE)
		bottle_flag = 1;
}

void alarm()
{
	if(bottle_flag == 1){
		ecrobot_sound_tone(900,512,30);
		alarm_flag = 1;
	}
	else
		alarm_flag = 0;
		
}

/*
 *	各種タスク
 */

//走行体管理タスク(4ms)
TASK(ActionTask)
{
	RN_modesetting();	//走行体状態設定
	taildown();			//尻尾制御
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
	logSend(cmd_forward,cmd_turn,get_sonar,bottle_flag,		//Bluetoothを用いてデータ送信
			alarm_flag,y_r);

	sonar();
	get_bottle();

	TerminateTask();
}



/******************************** END OF FILE ********************************/

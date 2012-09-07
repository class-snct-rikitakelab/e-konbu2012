/*
 * markerdetection.c
 * マーカー検出プログラム
 */


#include "markerDetector.h"


/*
 *	各種定義
*/

#define PI 3.141592

//尻尾設定角度
#define ANGLEOFDOWN 95				//降下目標角度
#define ANGLEOFUP 6					//上昇目標角度

//ジャイロ振幅値
#define PM_GYRO 65

/* 車輪半径、走行体幅*/
#define W_RADIUS	4.1	//[mm]
#define W_DIST		16.2//[mm]

//速度カウンタの上限値
#define SPEED_COUNT 5

//リモートスタートコマンド(変更禁止)
#define CMD_START '1'

/*
 *	グローバル変数
 */

//ライントレース用目標値
static unsigned int BLACK_VALUE;	//黒値
static unsigned int WHITE_VALUE;	//白値
static unsigned int GRAY_VALUE;		//灰色値（現在は黒と白の平均値

static unsigned int GYRO_OFFSET;


//目標速度
static S8 trgt_speed = 60;

//ライントレース時PID制御用係数
//前進量60時のPIDパラメータ
static float Kp = 1.85;				//P制御用
static float Ki = 1.6;				//I制御用
static float Kd = 0.003;			//D制御用

/*//前進量110時のPIDパラメータ
static float Kp = 0.456;			//P制御用
static float Ki = 1.61;				//I制御用
static float Kd = 0.017;			//D制御用
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

//ジャイロセンサオフセット計算用変数
static U32	gyro_offset = 0;    /* gyro sensor offset value */

/* 自己位置同定用変数 */
static float x_r = 0;		//車体のX座標
static float y_r = 0;		//車体のY座標
static float dist = 0;		//移動距離
static float theta = 0;		//車体の角度
static float R = 0;			//曲率半径

char rx_buf[BT_MAX_RX_BUF_SIZE];

/* バランスコントロールへ渡すコマンド用変数 */
S8  cmd_forward, cmd_turn;
/* バランスコントロールから返されるモータ制御用変数 */
S8	pwm_l, pwm_r;


//目標曲率半径
static float trgt_R = 0;

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
	RN_LOOKUP,					//ルックアップゲート準備
	RN_LOOKUPDOWN,				//走行体降下
	RN_LOOKUPMOVE,				//走行体前進
	RN_LOOKUPUP,				//走行体復帰
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
void logSend(S8 data1, S8 data2, S16 adc1, S16 adc2, S16 adc3, S16 adc4);
void taildown();
int RA_wheels(int turn);
void RN_modesetting();
static int remote_start(void);
void runner_mode_change(int flag);
int getsonarflag(int target_sonar);
void tailpower(float value);
void tail_mode_change(int mode,int value,int limit,int t_up);

void tail_run(S8 forward, S8 turn, S8 *pwm_l, S8 *pwm_r);

S8 RA_speedCtrl(S8 target);
S8 RA_linetrace_PID();
S8 RA_curvatureCtrl_PID(float target);

void self_location(void);
float deg2rad(float degree);
float rad2deg(float radian);

S8 markerDetector();

//カウンタの宣言
DeclareCounter(SysTimerCnt);

//液晶ディスプレイに表示するシステム名設定
const char target_subsystem_name[] = "ETrobotti";


/*
 *	関数実装
 */


//初期処理関数（プログラムの最初に呼び出し）
void ecrobot_device_initialize(void){
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
void ecrobot_device_terminate(void){
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


//輝度値PID制御ライントレース関数
S8 RA_linetrace_PID(){
	static float bf_dev = 0.0;
	static float i_dev  = 0.0;

	float dev = (float)GRAY_VALUE - (float)ecrobot_get_light_sensor(NXT_PORT_S3);
	
	i_dev = i_dev + (dev*0.0002);
	float d_dev = (dev - bf_dev)/0.0002;
	bf_dev = dev;

	S8 turn = -(Kp * dev + Ki * i_dev + Kd * d_dev);
	if (-100 > turn) {
		turn = -100;
	}
	else if (100 < turn) {
		turn = 100;
	}

	return turn;
}

//段階的加速用関数（指定量だけ速度を徐々に上昇）
S8 RA_speedCtrl(S8 target){
	static S8 crt_speed = 0;
	static int cnt = 0;

	if(cnt++ >= SPEED_COUNT){
		if(target - crt_speed >= 0){
			crt_speed++;
			if(crt_speed > target){
				crt_speed = target;
			}
		}
		else {
			crt_speed--;
			if(crt_speed < target){
				crt_speed = target;
			}
		}
		cnt = 0;
	}
	return crt_speed;
}

//2車輪の回転量差、P制御関数（目標値に近づける）
int RA_wheels(int turn){
	float w_kp = 1.4;	//回転量P制御パラメータ

	signed long def = ecrobot_get_motor_rev(NXT_PORT_B) - ecrobot_get_motor_rev(NXT_PORT_C);	//回転量差
	turn = w_kp * def;	//回転量算出

	return turn;
}

//曲率半径PID制御関数
S8 RA_curvatureCtrl_PID(float target) {
	static float bf_dev = 0.0;

	float dev = R - target;
	bf_dev = dev;

	S8 turn = 0.8 * dev;
	if (-100 > turn) {
		turn = -100;
	}
	else if (100 < turn) {
		turn = 100;
	}

	return turn;
}

//走行体モード変更関数（主にバランサーのON/OFF）
void runner_mode_change(int flag){
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
void tailpower(float value){
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
		if(t_angle <= t_value){		//現在の角度が目標値以下かどうか
			t_hensa = t_angle - ecrobot_get_motor_rev(NXT_PORT_A);
			if(t_count >= t_count_limit){	//カウンタで尻尾を下げる速度を調整
				t_angle+=t_up;			//角度を上げる
				t_count = 0;
			}
		}
		else{
			t_angle = t_value;
		}
		break;

	case(RN_TAILUP):				//尻尾を上げる
		if(t_angle >= t_value){		//現在の角度が目標値以上かどうか
			t_hensa = t_angle - ecrobot_get_motor_rev(NXT_PORT_A);
			if(t_count >= t_count_limit){	//カウンタで尻尾を上げる速度を調整
				t_angle-=t_up;			//角度を下げる
				t_count = 0;
			}
		}
		else{
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
void tail_mode_change(int mode,int value,int limit,int up){	//mode(0:尻尾を下ろす、1:尻尾を上げる) value(目標値) limit(上げるほど遅延） up（上げるほど加速）1
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
static int remote_start(void){
	int i;
	unsigned int rx_len;
	unsigned char start = 0;		//状態フラグ

	for (i=0; i<BT_MAX_RX_BUF_SIZE; i++){
		rx_buf[i] = 0; //受信バッファをクリア
	}

	rx_len = ecrobot_read_bt(rx_buf, 0, BT_MAX_RX_BUF_SIZE);
	if (rx_len > 0){
		//受信データあり
		if (rx_buf[0] == CMD_START){
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
void RN_setting(){
	static unsigned char buf = 0;
	if(ecrobot_get_touch_sensor(NXT_PORT_S4)){
		buf = 1;
	}
	if(buf && !ecrobot_get_touch_sensor(NXT_PORT_S4)){
		setting_mode = RN_SETTINGMODE_START;
		runner_mode  = RN_MODE_INIT;
		ecrobot_set_motor_rev(NXT_PORT_B, 0);
		ecrobot_set_motor_rev(NXT_PORT_C, 0);
		nxt_motor_set_speed(NXT_PORT_C, 0, 1);
		nxt_motor_set_speed(NXT_PORT_B, 0, 1);
		buf = 0;
	}

	switch (setting_mode){
	//キャリブレーション状態
	case (RN_SETTINGMODE_START):
		RN_calibrate();
		break;
		
	case (RN_SPEEDZERO):
		cmd_forward = 0;
		cmd_turn = RA_wheels(cmd_turn);
		wait_count++;
		if(wait_count >= 200){
			setting_mode = RN_RUN;
			wait_count = 0;
		}
		break;
	
	//通常走行状態
	case (RN_RUN):
		cmd_forward = RA_speedCtrl(trgt_speed);
		cmd_turn	= RA_linetrace_PID();
		//cmd_turn    = RA_curvatureCtrl_PID(trgt_R);
		//cmd_turn    = RA_linetrace_PID() + RA_curvatureCtrl_PID(0.0);
		wait_count++;
		break;

	default:
		break;
	}
}

//キャリブレーション関数
void RN_calibrate(){

	tail_mode_change(0,ANGLEOFDOWN,1,2);

	//黒値
	while(1){
		if(ecrobot_get_touch_sensor(NXT_PORT_S4) == TRUE){
			ecrobot_sound_tone(880, 512, 10);
			BLACK_VALUE=ecrobot_get_light_sensor(NXT_PORT_S3);
			systick_wait_ms(500);
			break;
		}
	}

	//白値
	while(1){
		if(ecrobot_get_touch_sensor(NXT_PORT_S4) == TRUE){
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
		if(ecrobot_get_touch_sensor(NXT_PORT_S4) == TRUE){
			ecrobot_sound_tone(932, 512, 10);
			gyro_offset += (U32)ecrobot_get_gyro_sensor(NXT_PORT_S1);
			GYRO_OFFSET = gyro_offset;
			systick_wait_ms(500);
			break;
		}
	}

	//走行開始合図
	while(1){
		//リモートスタート
		if(remote_start()==1){
			ecrobot_sound_tone(982,512,30);
//			tail_mode_change(1,ANGLEOFUP,0,2);
			setting_mode = RN_SPEEDZERO;
			runner_mode = RN_MODE_BALANCEOFF;
			break;
		}

		//タッチセンサスタート
		else if (ecrobot_get_touch_sensor(NXT_PORT_S4) == TRUE){
			ecrobot_sound_tone(982,512,10);
			while(1){
				if (ecrobot_get_touch_sensor(NXT_PORT_S4) != TRUE){
					setting_mode = RN_SPEEDZERO;
					runner_mode_change(2);
//					tail_mode_change(1,ANGLEOFUP,0,2);
					break;
				}
			}
			break;
		}
	}
}

//自己位置同定関数
void self_location(){
	static float dist_t = 0.0;
	static float theta_t = 0.0;

	float fi_l = (float)nxt_motor_get_count(NXT_PORT_C);
	float fi_r = (float)nxt_motor_get_count(NXT_PORT_B);
	float dist_l = deg2rad(fi_l * W_RADIUS);
	float dist_r = deg2rad(fi_r * W_RADIUS);

	dist = (dist_l + dist_r) / 2.0;
	theta = W_RADIUS / W_DIST * (fi_r - fi_l);
	x_r += (dist - dist_t) * sin(deg2rad(theta));
	y_r += (dist - dist_t) * cos(deg2rad(theta));
	if(!(theta == theta_t)){
		R = rad2deg((dist - dist_t) / (theta - theta_t));
	}
	else{
		R = 0.0;
	}
		
	dist_t = dist;
	theta_t = theta;
}

float deg2rad(float degree){
	float radian = PI / 180.0 * degree;
	return radian;
}

float rad2deg(float radian){
	float degree = 180.0 / PI * radian;
	return degree;
}

//走行体状態設定関数
void RN_modesetting(){
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
		nxt_motor_set_speed( NXT_PORT_C, pwm_l, 1);
		nxt_motor_set_speed( NXT_PORT_B, pwm_r, 1);
		break;
		
	//バランサー無し
	case (RN_MODE_BALANCEOFF):
		tail_run( cmd_forward, cmd_turn, &pwm_l, &pwm_r);
		nxt_motor_set_speed( NXT_PORT_C, pwm_l, 1);
		nxt_motor_set_speed( NXT_PORT_B, pwm_r, 1);
		break;

	default:
		nxt_motor_set_speed( NXT_PORT_C, 0, 1);
		nxt_motor_set_speed( NXT_PORT_B, 0, 1);
		break;
	}
}

//しっぽ走行用左右サーボモータpwm値算出
void tail_run(S8 forward, S8 turn, S8 *pwm_l, S8 *pwm_r){
	S16 temp;

	temp = forward + turn/2;
	if (-128 > temp) {
		*pwm_l = -128;
	}
	else if (127 < temp) {
		*pwm_l = 127;
	}
	else {
		*pwm_l = temp;
	}
	
	temp = forward - turn/2;
	if (-128 > temp) {
		*pwm_r = -128;
	}
	else if (127 < temp) {
		*pwm_r = 127;
	}
	else {
		*pwm_r = temp;
	}
}

//マーカー検知 v2
S8 markerDetector(){
	//輝度値の変化の二回微分
	static S16 LV_buf = 0;
	static S8 dev_buf[5]  = { 0, 0, 0, 0, 0};
	static S8 dev2_buf[2] = { 0, 0};
	
	S16 LV = ecrobot_get_light_sensor(NXT_PORT_S3);
	S8 dev = LV - LV_buf;
	S8 dev2 = dev - dev_buf[0];

	//マーカー検知アルゴリズム
	S8 mkflg = 0;
	if(dev2_buf[0] <= -15 && (dev2_buf[1] >= 5 || dev2 >= 5)){
		mkflg = 1;
	}
	else {
		mkflg = 0;
	}

	LV_buf		= LV;
	dev_buf[4]	= dev_buf[3];
	dev_buf[3]	= dev_buf[2];
	dev_buf[2]	= dev_buf[1];
	dev_buf[1]	= dev_buf[0];
	dev_buf[0]	= dev;
	dev2_buf[1] = dev2_buf[0];
	dev2_buf[0] = dev2;
	
	return mkflg;
}


/*
 *	各種タスク
 */

//走行体管理タスク(4ms)
TASK(ActionTask){
	RN_modesetting();	//走行体状態設定
	taildown();			//尻尾制御
	TerminateTask();
}

//走行状態管理タスク(5ms)
TASK(ActionTask2){
	RN_setting();		//走行状態設定
	TerminateTask();
}

//状態表示管理タスク(20ms)
TASK(DisplayTask){
	ecrobot_status_monitor(target_subsystem_name);	//モニタに状態表示
	TerminateTask();
}

//ログ送信、超音波センサ管理タスク(50ms) (共に50msでなければ動作しない）
TASK(LogTask){
	//Bluetoothを用いてデータ送信
	logSend(cmd_forward, cmd_turn, (S16)x_r, (S16)y_r, (S16)theta, (S16)R);
	self_location();
	if(systick_get_ms() > 2000 && markerDetector()){
		ecrobot_sound_tone( 800, 100, 40);
	}
	TerminateTask();
}



/******************************** END OF FILE ********************************/

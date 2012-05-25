/**
 ******************************************************************************
 **	ファイル名 : sample.c
 **
 **	概要 : 2輪倒立振子ライントレースロボットのTOPPERS/ATK1(OSEK)用Cサンプルプログラム
 **
 ** 注記 : sample_c4 (sample_c3にBluetooth通信リモートスタート機能を追加)
 ******************************************************************************
 **/
#include "kernel.h"
#include "kernel_id.h"
#include "ecrobot_interface.h"
#include "balancer.h" /* 倒立振子制御用ヘッダファイル */
#include "logSend.h"
//#include "Maimai.h"
#include <stdlib.h>

/* 下記のマクロは個体/環境に合わせて変更する必要があります */
/* sample_c1マクロ */
#define GYRO_OFFSET  600 /* ジャイロセンサオフセット値(角速度0[deg/sec]時) */
#define LIGHT_WHITE	 500 /* 白色の光センサ値 */
#define LIGHT_BLACK	 700 /* 黒色の光センサ値 */
/* sample_c2マクロ */
#define SONAR_ALERT_DISTANCE 30 /* 超音波センサによる障害物検知距離[cm] */
/* sample_c3マクロ */
#define TAIL_ANGLE_STAND_UP 107 /* 完全停止時の角度[度] */
#define TAIL_ANGLE_DRIVE      3 /* バランス走行時の角度[度] */
#define P_GAIN             2.5F /* 完全停止用モータ制御比例係数 */
#define PWM_ABS_MAX          60 /* 完全停止用モータ制御PWM絶対最大値 */
/* sample_c4マクロ */
#define DEVICE_NAME       "KINGYO"  /* Bluetooth通信用デバイス名 */
#define PASS_KEY          "1234" /* Bluetooth通信用パスキー */
#define CMD_START         '1'    /* リモートスタートコマンド(変更禁止) */

//Macro for PID 
#define MOKUHYOU 600
#define DELTA 0.004

#define LIGHT_THRESHOLD .7F

#define VOL 20

/* 関数プロトタイプ宣言 */
static int sonar_alert(void);
static void tail_control(signed int angle);
static int remote_start(void);
static void Maimai(void);

/* Bluetooth通信用データ受信バッファ */
char rx_buf[BT_MAX_RX_BUF_SIZE];
int rx_buf_int[BT_MAX_RX_BUF_SIZE];



//まいまい式用グローバル変数　目標値
float brightness;
//*****************************************************************************
// 関数名 : ecrobot_device_initialize
// 引数 : なし
// 戻り値 : なし
// 概要 : ECROBOTデバイス初期化処理フック関数
//*****************************************************************************
void ecrobot_device_initialize()
{
	ecrobot_set_light_sensor_active(NXT_PORT_S3); /* 光センサ赤色LEDをON */
	ecrobot_init_sonar_sensor(NXT_PORT_S2); /* 超音波センサ(I2C通信)を初期化 */
	nxt_motor_set_count(NXT_PORT_A, 0); /* 完全停止用モータエンコーダリセット */
	ecrobot_init_bt_slave(PASS_KEY); /* Bluetooth通信初期化 */
}

//*****************************************************************************
// 関数名 : ecrobot_device_terminate
// 引数 : なし
// 戻り値 : なし
// 概要 : ECROBOTデバイス終了処理フック関数
//*****************************************************************************
void ecrobot_device_terminate()
{
	ecrobot_set_light_sensor_inactive(NXT_PORT_S3); /* 光センサ赤色LEDをOFF */
	ecrobot_term_sonar_sensor(NXT_PORT_S2); /* 超音波センサ(I2C通信)を終了 */
	ecrobot_term_bt_connection(); /* Bluetooth通信を終了 */
}

//*****************************************************************************
// 関数名 : user_1ms_isr_type2
// 引数 : なし
// 戻り値 : なし
// 概要 : 1msec周期割り込みフック関数(OSEK ISR type2カテゴリ)
//*****************************************************************************
void user_1ms_isr_type2(void){}

//*****************************************************************************
// タスク名 : TaskMain
// 概要 : メインタスク
//*****************************************************************************


TASK(TaskMain)
{
	signed char forward;   /* 前後進命令 */
	signed char turn;         /* 旋回命令 */
	signed char pwm_L, pwm_R; /* 左右モータPWM出力 */
	
	//PID制御用変数群
	static float Kp = 0.648;
	static float Ki = 0.35;
	static float Kd = 0.0205;
	
	static float now_diff = 0;
	static float before_diff = 0;
	static float integral = 0;
	
	static float gyro_offset = 600;  /* ジャイロセンサオフセット値(角速度0[deg/sec]時) */
	static float  light_white = 500; /* 白色の光センサ値 */
static float light_black =700; /* 黒色の光センサ値 */
	//ログ送信用変数
		S8 data1=0;
		S8 data2=0;
		S16 adc1=0;
		S16 adc2=0;
		S16 adc3=0;
		S16 adc4=0;
		S32 valOfDistance=0;
		
	/**
	 * Bluetooth通信用デバイス名の変更は、Bluetooth通信接続が確立されていない場合のみ有効です。
	 * 通信接続確立時にはデバイス名は変更されません。(下記のAPIは何もしません)
	 */
	ecrobot_set_bt_device_name(DEVICE_NAME);

	//キャリブレーション
	while(1){ //黒色
			if(ecrobot_get_touch_sensor(NXT_PORT_S4)){
				ecrobot_sound_tone(440, 512, VOL);
				systick_wait_ms(10);
				ecrobot_sound_tone(369, 256, VOL);
				systick_wait_ms(10);
				ecrobot_sound_tone(880, 512, VOL);
				systick_wait_ms(10);
				light_black = ecrobot_get_light_sensor(NXT_PORT_S3);
				systick_wait_ms(300);
				break;
			}
		}
		while(1){ //白色
			if(ecrobot_get_touch_sensor(NXT_PORT_S4)){
				ecrobot_sound_tone(440, 512, VOL);
				systick_wait_ms(10);
				ecrobot_sound_tone(369, 256, VOL);
				systick_wait_ms(10);
				ecrobot_sound_tone(880, 512, VOL);
				systick_wait_ms(10);
				light_white = ecrobot_get_light_sensor(NXT_PORT_S3);
				systick_wait_ms(300);
				break;
			}
		}

		while(1){ //ジャイロオフセット
			if(ecrobot_get_touch_sensor(NXT_PORT_S4)){
				gyro_offset = ecrobot_get_gyro_sensor(NXT_PORT_S1);		
				systick_wait_ms(300);
				ecrobot_sound_tone(440, 512, VOL);
				systick_wait_ms(10);
				ecrobot_sound_tone(369, 256, VOL);
				systick_wait_ms(10);
				ecrobot_sound_tone(880, 512, VOL);
				systick_wait_ms(10);
				break;
			}
		}
	
	
	while(1)
	{
		tail_control(TAIL_ANGLE_STAND_UP); /* 完全停止用角度に制御 */
  
	
		if (remote_start() == 1)
		{
			break; /* リモートスタート */
		}

		if (ecrobot_get_touch_sensor(NXT_PORT_S4) == 1)
		{
			systick_wait_ms(10);
			break; /* タッチセンサが押された */
		}

		systick_wait_ms(10); /* 10msecウェイト */
	}

	balance_init();						/* 倒立振子制御初期化 */
	nxt_motor_set_count(NXT_PORT_C, 0); /* 左モータエンコーダリセット */
	nxt_motor_set_count(NXT_PORT_B, 0); /* 右モータエンコーダリセット */
	
//	int buf[100];
		U32 off=0;
		U32 len;
	U8 *buf;
	U32 recv_byte;
rx_buf[0]=5;
	buf[1]=10;
   int  i;
	for (i=0; i<BT_MAX_RX_BUF_SIZE; i++)
	{
		rx_buf[i] = '0'; /* 受信バッファをクリア */
	}
	
	/*
	while(1) {
		recv_byte=//ecrobot_read_bt_packet(buf,len);
		ecrobot_read_bt(rx_buf,off,len);
		if(recv_byte>0){
			
			ecrobot_sound_tone(440, 512, VOL);
				systick_wait_ms(10);
				ecrobot_sound_tone(369, 256, VOL);
				systick_wait_ms(10);
				ecrobot_sound_tone(880, 512, VOL);
				systick_wait_ms(100);
		}
		tail_control(TAIL_ANGLE_STAND_UP); // 完全停止用角度に制御
		nxt_motor_set_speed(NXT_PORT_C, (rx_buf[0]-'0')*10, 1); // 左モータPWM出力セット(-100～100)
		nxt_motor_set_speed(NXT_PORT_B,(rx_buf[1]-'0')*10, 1); // 右モータPWM出力セット(-100～100)
	//break;	
		
		
		
		
	}	
		while(1) {
		
		tail_control(TAIL_ANGLE_STAND_UP); // 完全停止用角度に制御
		nxt_motor_set_speed(NXT_PORT_C, buf[0], 1); // 左モータPWM出力セット(-100～100)
		nxt_motor_set_speed(NXT_PORT_B, buf[0], 1); // 右モータPWM出力セット(-100～100)
	//break;	
		
		
		
	}	
	
	*/
	float mokuhyou_val;
	 unsigned int counter_maimai=0;

	//通常走行
	 /*
	while(1){

		++counter_maimai;
if(counter_maimai==20/4){//約20ms
	//赤色LEDを点灯する
	ecrobot_set_light_sensor_active(NXT_PORT_S3);
	}

if(counter_maimai==40/4){//約40ms
	//赤色LEDを消灯する
	ecrobot_set_light_sensor_inactive(NXT_PORT_S3);
	//カウンタをクリア
	counter_maimai=0;
	}
systick_wait_ms(4); // 4msecウェイト 
	}
	*/
	while(1)
	{
		tail_control(TAIL_ANGLE_DRIVE); /* バランス走行用角度に制御 */

		if (sonar_alert() == 1) /* 障害物検知 */
		{
			forward = turn = 0; /* 障害物を検知したら停止 */
		}
		else
		{
			forward = 20; /* 前進命令 */
			//まいまい式とPID制御を組み合わせたturn値の計算	
			/*mokuhyou_val =*/Maimai();

			if(brightness<=LIGHT_THRESHOLD/*0.6*light_black+0.4*light_white*/){
				turn =20;
			}
			else{
				turn =-20;

			}

			/*
			before_diff = now_diff;
			now_diff = ((0.6*light_black+0.4*light_white)+mokuhyou_val)- ecrobot_get_light_sensor(NXT_PORT_S3);
			integral = integral + ((now_diff + before_diff)/2.0 * DELTA);
			
			turn = Kp*now_diff + Ki*integral + Kd*((now_diff - before_diff) / DELTA);
			*/

			/* 通常のPID制御
			before_diff = now_diff;
			now_diff = (0.6*light_black+0.4*light_white) - ecrobot_get_light_sensor(NXT_PORT_S3);
			integral = integral + ((now_diff + before_diff)/2.0 * DELTA);
			
			turn = Kp*now_diff + Ki*integral + Kd*((now_diff - before_diff) / DELTA);
			*/
			/*
			if(turn > 100){
				turn = 100;
			}
			else if(turn < -100){
				turn = -100;
			}
			*/
		}
	
		
		
		// bluetooth send data
		//adc1 = (S16)ecrobot_get_gyro_sensor(NXT_PORT_S1);
		//adc2 =(S16)ecrobot_get_battery_voltage();
		
	//	logSend(data1,data2,adc1,adc2,adc3,adc4);
		
		
		
		
		
		
	
		/* 倒立振子制御(forward = 0, turn = 0で静止バランス) */
		balance_control(
			(float)forward,								 /* 前後進命令(+:前進, -:後進) */
			(float)turn,								 /* 旋回命令(+:右旋回, -:左旋回) */
			(float)ecrobot_get_gyro_sensor(NXT_PORT_S1), /* ジャイロセンサ値 */
			(float)gyro_offset,							 /* ジャイロセンサオフセット値 */
			(float)nxt_motor_get_count(NXT_PORT_C),		 /* 左モータ回転角度[deg] */
			(float)nxt_motor_get_count(NXT_PORT_B),		 /* 右モータ回転角度[deg] */
			(float)ecrobot_get_battery_voltage(),		 /* バッテリ電圧[mV] */
			&pwm_L,										 /* 左モータPWM出力値 */
			&pwm_R);									 /* 右モータPWM出力値 */
		nxt_motor_set_speed(NXT_PORT_C, pwm_L, 1); /* 左モータPWM出力セット(-100～100) */
		nxt_motor_set_speed(NXT_PORT_B, pwm_R, 1); /* 右モータPWM出力セット(-100～100) */
		 
		systick_wait_ms(4); /* 4msecウェイト */
	
		
	}



}

//*****************************************************************************
// 関数名 : sonar_alert
// 引数 : 無し
// 返り値 : 1(障害物あり)/0(障害物無し)
// 概要 : 超音波センサによる障害物検知
//*****************************************************************************
static int sonar_alert(void)
{
	static unsigned int counter = 0;
	static int alert = 0;

	signed int distance;

	if (++counter == 40/4) /* 約40msec周期毎に障害物検知  */
	{
		/* v
		 * 超音波センサによる距離測定周期は、超音波の減衰特性に依存します。
		 * NXTの場合は、40msec周期程度が経験上の最短測定周期です。
		 */
		distance = ecrobot_get_sonar_sensor(NXT_PORT_S2);
		if ((distance <= SONAR_ALERT_DISTANCE) && (distance >= 0))
		{
			alert = 1; /* 障害物を検知 */
		}
		else
		{
			alert = 0; /* 障害物無し */
		}
		counter = 0;
	}

	return alert;
}

//*****************************************************************************
// 関数名 : tail_control
// 引数 : angle (モータ目標角度[度])
// 返り値 : 無し
// 概要 : 走行体完全停止用モータの角度制御
//*****************************************************************************
static void tail_control(signed int angle)
{
	float pwm = (float)(angle - nxt_motor_get_count(NXT_PORT_A))*P_GAIN; /* 比例制御 */
	/* PWM出力飽和処理 */
	if (pwm > PWM_ABS_MAX)
	{
		pwm = PWM_ABS_MAX;
	}
	else if (pwm < -PWM_ABS_MAX)
	{
		pwm = -PWM_ABS_MAX;
	}

	nxt_motor_set_speed(NXT_PORT_A, (signed char)pwm, 1);
}

//*****************************************************************************
// 関数名 : remote_start
// 引数 : 無し
// 返り値 : 1(スタート)/0(待機)
// 概要 : Bluetooth通信によるリモートスタート。 Tera Termなどのターミナルソフトから、
//       ASCIIコードで1を送信すると、リモートスタートする。
//*****************************************************************************
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


void Maimai(){
static unsigned int counter_maimai=0;
U16 lightDowned;
U16 lightUpped;
U16 lightDiff;
float k;

//static float brighteness,buf_brighness;

++counter_maimai;
if(counter_maimai==20/4){//約20ms
	//赤色LED消灯時のセンサ値を取得
	lightDowned=ecrobot_get_light_sensor(NXT_PORT_S3);
	//赤色LEDを点灯する
	ecrobot_set_light_sensor_active(NXT_PORT_S3);
	}

if(counter_maimai==40/4){//約40ms
	//赤色LED点灯時のセンサ値を取得
	lightUpped=ecrobot_get_light_sensor(NXT_PORT_S3);
	//赤色LEDを消灯する
	ecrobot_set_light_sensor_inactive(NXT_PORT_S3);
	//カウンタをクリア
	counter_maimai=0;
}

//光センサの変化量を計算

if(lightDowned - lightUpped>0){
	lightDiff=lightDowned - lightUpped;
}
else{
	lightDiff=0U;
}

k=(1.0382E-3 * lightDowned -6.3295E-1)*lightDowned + 1.1024E+2;
brightness = (float)lightDiff/k;

}
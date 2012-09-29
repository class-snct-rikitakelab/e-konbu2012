#include "positionPresumption.h"

/*
 * markerdetection.c
 * マーカー検出プログラム
 */


#include "positionPresumption.h"
#include "math.h"


/*
 *	各種定義
*/

#define PI 3.141592

//尻尾設定角度
#define ANGLEOFDOWN 112				//降下目標角度
#define ANGLEOFUP 6					//上昇目標角度

//速度カウンタの上限値
#define SPEED_COUNT 20

//ジャイロ振幅値
#define PM_GYRO 65

/* 車輪半径、走行体幅*/
#define W_RADIUS	4.1	//[mm]
#define W_DIST		16.2//[mm]

//リモートスタートコマンド(変更禁止)
#define CMD_START '1'

/*
 *	グローバル変数
 */

//ライントレース用目標値
static unsigned int BLACK_VALUE;	//黒値
static unsigned int WHITE_VALUE;	//白値
static unsigned int GRAY_VALUE;		//灰色値（現在は黒と白の平均値）
//static unsigned int LOOKUP_BLACK_VALUE;		//角度がANGLEOFLOOKUP時の黒値
//static unsigned int LOOKUP_WHITE_VALUE;		//角度がANGLEOFLOOKUP時の白値
static unsigned int LOOKUP_GRAY_VALUE;		//角度がANGLEOFLOOKUP地の灰色値（現在は黒と白の平均値）

//static unsigned int LOOKUP_SONAR_VALUE;

//速度調節カウンタ（カウンタが上限に達する毎に速度が1上昇
static int speed_counter = 0;


//PID制御用偏差値
static float hensa;					//P制御用
static float i_hensa = 0;			//I制御用
static float d_hensa = 0;			//D制御用
static float bf_hensa = 0;


//ライントレース時PID制御用係数
static float Kp = 1.85;				//P制御用
static float Ki = 1.6;				//I制御用
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

//ジャイロセンサオフセット計算用変数
//static U32	gyro_offset = 0;    /* gyro sensor offset value */

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

//目標速度
static S8 trgt_speed = 100;


//目標曲率半径
static float trgt_R = 0;

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


//キャリブレーションの状態
typedef enum{
	RN_SETTINGMODE_START,		//初期状態
	RN_SPEEDZERO,				//速度ゼロキープ
	RN_RUN,						//基本走行（ライントレース）
	RN_LOOKUP,					//ルックアップゲート準備
	RN_LOOKUPDOWN,				//走行体降下
	RN_LOOKUPMOVE,				//走行体前進
	RN_LOOKUPUP,				//走行体復帰
	RN_CVRUN,					//曲率PID
	RN_LOT90					//90度旋回（階段）
} RN_SETTINGMODE;

//尻尾の状態
typedef enum{
	RN_TAILDOWN,				//尻尾降下
	RN_TAILUP,					//尻尾上昇
} RN_TAILMODE;

/* 走行区間 */
typedef enum{
	START,
	UP_SLOPE,
	DOWN_SLOPE,
	FST_CORNER,
	FST_STRAIGHT,
	SND_CORNER,
	SND_STRAIGHT,
	TRD_CORNER,
	TRD_STRAIGHT,
	FIN_APPROACH,
	B_FST_STRAIGHT,
	B_FST_CORNER,
	B_SND_STRAIGHT,
	B_SND_CORNER,
	B_TRD_STRAIGHT,
	B_TRD_CORNER,
	LOOKUP
} IN_SECTION;

//初期状態
RN_MODE runner_mode = RN_MODE_INIT;
RN_SETTINGMODE setting_mode = RN_SETTINGMODE_START;
RN_TAILMODE tail_mode = RN_TAILUP;

/*
 *	各種プライベート関数定義
 */


//各種プライベート関数
void RN_setting();

void RN_modesetting();


int RA_directionCtrl_PID(float target);
int RA_curvatureCtrl_PID(float target);

//void self_location(void);
//void setSection();void changeSection(float *buf_x, float *buf_y, float *buf_l, float *buf_th);
//unsigned char MKTrigger(void);
//signed char LVTrigger(void);
int abs(int n);
float deg2rad(float degree);
float rad2deg(float radian);

//曲率半径PID制御関数
int RA_curvatureCtrl_PID(float target) {
	static float bf_dev = 0.0;

	float dev = R - target;
	//float i_dev = i_dev + (dev * 0.0005);
	//float d_dev = (dev - bf_dev) / 0.0005;
	bf_dev = dev;

	//S8 turn = Kp * dev + Ki * i_dev + Kd * d_dev;
	int turn = 0.6 * dev;
	if (-100 > turn) {
		turn = -100;
	}
	else if (100 < turn) {
		turn = 100;
	}

	return turn;
}

//車体方向PID制御関数
int RA_directionCtrl_PID(float target) {
	static float bf_dev = 0.0;
	
	float dev = theta - target;
	//float i_dev = i_dev + (dev * 0.0005);
	float d_dev = (dev - bf_dev) / 0.0005;
	bf_dev = dev;
	
	//S8 turn = Kp * dev + Ki * i_dev + Kd * d_dev;
	int turn = 1.0 * dev + 0.5 * d_dev;
	if (-100 > turn) {
		turn = -100;
	}
	else if (100 < turn) {
		turn = 100;
	}

	return turn;
}


//走行状態設定関数
void RN_setting(){

	switch (setting_mode){
	//キャリブレーション状態
	case (RN_SETTINGMODE_START):
		RN_calibrate();
		LV_buf = ecrobot_get_light_sensor(NXT_PORT_S3);
		break;
		
	//通常走行状態
	case (RN_RUN):
		RA_linetrace_PID(SPEED_COUNT);
		wait_count++;
		break;
	
	// 曲率PID
	case (RN_CVRUN):
		cmd_forward = SPEED_COUNT;
		cmd_turn = RA_curvatureCtrl_PID(trgt_R);
		wait_count++;
		break;
		
	case (RN_LOT90):
		cmd_turn = 0;
		cmd_turn = RA_directionCtrl_PID(90);
		wait_count++;
		break;

	default:
		break;
	}
}

/*
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
*/
float deg2rad(float degree){
	float radian = PI / 180.0 * degree;
	return radian;
}

float rad2deg(float radian){
	float degree = 180.0 / PI * radian;
	return degree;
}

/* 走行区間 */
typedef enum{
	START,
	UP_SLOPE,
	DOWN_SLOPE,
	FST_CORNER,
	FST_STRAIGHT,
	SND_CORNER,
	SND_STRAIGHT,
	TRD_CORNER,
	TRD_STRAIGHT,
	FIN_APPROACH,
	B_FST_STRAIGHT,
	B_FST_CORNER,
	B_SND_STRAIGHT,
	B_SND_CORNER,
	B_TRD_STRAIGHT,
	B_TRD_CORNER,
	LOOKUP
} IN_SECTION;

/* 走行区間検出 */
/*
void setSection(){
	static IN_SECTION crt_sect = START;
	
	static float buf_l = 0.0;
	float def_l  = dist - buf_l;

	switch(crt_sect){
	case (START):			//スタート→坂道
		if(GYRO_OFFSET - 30 > (U32)ecrobot_get_gyro_sensor(NXT_PORT_S1) && wait_count > 500){
			cngSection(&crt_sect, &buf_l, 220);
		}
		trgt_R = 0.0;
		break;
	case (UP_SLOPE):		//坂道始点→頂点
		if(def_l >= 30){
			cngSection(&crt_sect, &buf_l, 233);
		}
		trgt_R = 0.0;
		break;
	case (DOWN_SLOPE):		//頂点→坂道終点
		//if(dist >= 390){
		if(def_l >= 90){
			cngSection(&crt_sect, &buf_l, 246);
		}
		trgt_speed = 60;
		Kp = 1.85;			//P制御用
		Ki = 1.6;			//I制御用
		Kd = 0.003;			//D制御用

		trgt_R = 0.0;
		break;
	case (FST_CORNER):		//坂道終点→第一カーブ
		if(def_l >= 110){
			cngSection(&crt_sect, &buf_l, 261);
		}
		trgt_speed = 100;
		Kp = 0.456;			//P制御用
		Ki = 1.61;			//I制御用
		Kd = 0.017;			//D制御用
		trgt_R = 67.59;
		break;
	case (FST_STRAIGHT):	//第一カーブ終点→第一ストレート
		if(def_l >= 115){
			cngSection(&crt_sect, &buf_l, 277);
		}
		trgt_R = 0.0;
		break;
	case (SND_CORNER):		//第一ストレート終点→第二カーブ
		if(def_l >= 245){
			cngSection(&crt_sect, &buf_l, 293);
		}
		trgt_R = 56.59;
		break;
	case (SND_STRAIGHT):	//第二カーブ終点→第二ストレート
		if(def_l >= 40){
			cngSection(&crt_sect, &buf_l, 311);
		}
		trgt_R = 0.0;
		break;
	case (TRD_CORNER):		//第二ストレート終点→第三カーブ
		if(def_l >= 235){
			cngSection(&crt_sect, &buf_l, 329);
		}
		trgt_R = -64.02;
		break;
	case (TRD_STRAIGHT):	//第三カーブ終点→第三ストレート
		if(def_l >= 115){
			cngSection(&crt_sect, &buf_l, 349);
		}
		trgt_R = 0.0;
		break;
	case (FIN_APPROACH):	//第三ストレート終点→マーカー
		if(def_l >= 66){
			cngSection(&crt_sect, &buf_l, 369);
		}
		trgt_R = 51.80;
		break;
	//以下、ボーナスステージ
	case (B_FST_STRAIGHT):
		if(def_l >= 64){
			cngSection(&crt_sect, &buf_l, 220);
		}
		trgt_speed = 60;
		Kp = 1.85;			//P制御用
		Ki = 1.6;			//I制御用
		Kd = 0.003;			//D制御用

		trgt_R = 0.0;
		break;
	case (B_FST_CORNER):
		if(def_l >= 70){
			cngSection(&crt_sect, &buf_l, 233);
		}
		trgt_R = 30.0;
		break;
	case (B_SND_STRAIGHT):
		if(def_l >= 66){
			cngSection(&crt_sect, &buf_l, 246);
		}
		trgt_R = 0.0;
		break;
	case (B_SND_CORNER):
		if(def_l >= 44){
			cngSection(&crt_sect, &buf_l, 261);
		}
		trgt_R = -30.0;
		break;
	case (B_TRD_STRAIGHT):
		if(def_l >= 107){
			cngSection(&crt_sect, &buf_l, 277);
		}
		trgt_R = 0.0;
		break;
	case (B_TRD_CORNER):
		if(def_l >= 21){
			cngSection(&crt_sect, &buf_l, 293);
		}
		trgt_R = -30.0;
		break;
	case (LOOKUP):
		if(def_l >= 203){
			cngSection(&crt_sect, &buf_l, 311);
		}
		trgt_R = 0.0;
		break;

	default:
		trgt_speed = 0;
		trgt_R = 0.0;
		break;
	}
}
*/
/*
void cngSection(IN_SECTION *crt_sect, float *buf_l, int tone){
	ecrobot_sound_tone(tone, 100, 50);
	*crt_sect += 1;
	*buf_l = dist;
}
*/
/*
void changeSection(float *buf_x, float *buf_y, float *buf_l, float *buf_th){
	*buf_x = x_r;	*buf_y = y_r;
	*buf_l = dist;	*buf_th = theta;
}
*/
/******************************** END OF FILE ********************************/

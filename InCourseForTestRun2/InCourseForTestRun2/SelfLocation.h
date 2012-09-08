#ifndef _SELFLOCATION_H_
#define _SELFLOCATION_H_

#include "math.h"
#include "ecrobot_interface.h"

/* 車輪半径、走行体幅*/
#define W_RADIUS	4.1	//[mm]
#define W_DIST		16.2//[mm]


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

static float x_r_zero = 0;	//X座標初期値
static float y_r_zero = 0;	//Y座標初期値
static float theta_R_zero = 0;	//車体角度初期値

static float dist = 0;		//移動距離
static float theta = 0;		//車体の角度
static float R = 0;			//曲率半径

extern void selflocation(void);

#endif


#ifndef _RUNNER_H_
#define _RUNNER_H_

#include "kernel.h"
#include "kernel_id.h"
#include "ecrobot_interface.h"
#include "balancer.h"

//ジャイロ振幅値
#define PM_GYRO 65
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
	MARKER,
	STEPS,
	TURN90D
} OUT_SECTION;

static OUT_SECTION crt_sect = START;

//目標曲率半径
static float trgt_R = 0;

//マーカーフラグ　0: OFF, 1: ON
unsigned char m_flg = 0;
static unsigned int LV_buf = 0;		/* Light Value buffer */


static float buf_x = 0.0, buf_y = 0.0, buf_l = 0.0, buf_th = 0.0; //区間推定用バッファ


extern S8 trgt_speed = 80;

extern S8 RA_directionCtrl_PID(float target);//車体角度が目標角度になるように旋回量を計算
extern S8 RA_curvatureCtrl_PID(float target);//引数で渡された目標曲率に近づくように旋回量を計算

void setLocation(void);
extern void setSection_out(void);

unsigned char MKTrigger(void);
signed char LVTrigger(void);


void changeSection(float *buf_x, float *buf_y, float *buf_l, float *buf_th);

//違いが不明


extern float getTargetR(); //現在区間の目標曲率を取得
extern S8 getTargSpeed();
extern void bufClear(); //区間推定用バッファのクリア
#endif

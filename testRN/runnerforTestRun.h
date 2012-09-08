
#ifndef _RUNNER_H_
#define _RUNNER_H_

#include "kernel.h"
#include "kernel_id.h"
#include "ecrobot_interface.h"
#include "balancer.h"

//ジャイロ振幅値
#define PM_GYRO 65


static unsigned int GYRO_OFFSET;


//速度カウンタの上限値
static int SPEED_COUNT = 60;

//速度調節カウンタ（カウンタが上限に達する毎に速度が1上昇
static int speed_counter = 0;






//目標曲率半径
static float trgt_R = 0;

//マーカーフラグ　0: OFF, 1: ON
unsigned char m_flg = 0;
static unsigned int LV_buf = 0;		/* Light Value buffer */



/* インコース走行区間 */
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
	LOOKUP
} SECT_IN;

/* アウトコース走行区間 */
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
	90DTURN
} SECT_OUT;



extern S8 RA_directionCtrl_PID(float target);　//車体角度が目標角度になるように旋回量を計算
extern S8 RA_curvatureCtrl_PID(float target);　//引数で渡された目標曲率に近づくように旋回量を計算

void setLocation(void);
void setSection_in(void);
void setSection_out(void);
void cngSection(float *buf_x, float *buf_y, float *buf_l, float *buf_th);
unsigned char MKTrigger(void);
signed char LVTrigger(void);

extern float getTartgetR(); //現在区間の目標曲率を取得

#endif
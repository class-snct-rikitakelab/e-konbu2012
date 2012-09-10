//#include "MainRunningInCourse.h"
#ifndef _TAILCONTROL_H_
#define _TAILCONTROL_H_

#include "kernel.h"
#include "kernel_id.h"
#include "ecrobot_interface.h"
#include "balancer.h"
#include "math.h"

#define ANGLEOFDOWN		90	//尻尾走行
#define ANGLEOFZERO		0
#define ANGLEOFUP		6	//尻尾不使用
#define ANGLEOFLOOKUP	56	//ルックアップゲート通過
#define ANGLEOFSTAND	100	//直立状態
#define ANGLEOFPUSH		120	//走行体跳ね上げ

#define TAIL_ANGLE_COUNT 5

//尻尾の状態
typedef enum{
	RN_TAILDOWN,				//尻尾降下
	RN_TAILUP,					//尻尾上昇
	RN_TAILLOOKUP,
	RN_TAILSTAND,
	RN_TAILPUSH,
} RN_TAILMODE;

//尻尾PI制御用係数
static float t_Kp = 8.85;			//P制御用
static float t_Ki = 0;				//I制御用

static int result_angle = 0;

RN_TAILMODE controltailmode = ANGLEOFZERO;

extern void TailControl(void);
extern void TargetTailAngleControl(void);
extern void TailModeChange(RN_TAILMODE tail_mode);

#endif

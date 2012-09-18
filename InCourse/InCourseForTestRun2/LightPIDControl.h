#ifndef _LIGHTPIDCONTROL_H_
#define _LIGHTPIDCONTROL_H_

#include "ecrobot_interface.h"
#include "Calibration.h"

//ライントレース時PID制御用係数
static float Kp =0.536;//1.53;//以前までの1.8;//0.536;//1.836;//0.436;//0.436;				//P制御用
static float Ki = 0.6;//1.2;//以前までの0.6;//1.6;//1.31;//0;//1.31;					//I制御用
static float Kd = 0.06;//0.006;//0.3;//以前までの0.006;//0.27;//0.5;//0.074;//0.011;				//D制御用

//PID制御用偏差値
static float hensa;					//P制御用
static float i_hensa = 0;			//I制御用
static float d_hensa = 0;			//D制御用
static float bf_hensa = 0;

static int graybase = 0;

extern int RA_linetrace_PID(int forward_speed);
extern void RA_hensareset();
extern int RA_linetrace();
extern int online();

extern void changeGray();

#endif

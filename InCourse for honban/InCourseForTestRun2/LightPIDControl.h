#ifndef _LIGHTPIDCONTROL_H_
#define _LIGHTPIDCONTROL_H_

#include "ecrobot_interface.h"
#include "Calibration.h"

static float Kp =0.836;//P制御用
static float Ki = 0.5;	//I制御用
static float Kd = 0.09;//D制御用

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

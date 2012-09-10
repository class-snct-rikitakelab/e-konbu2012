#ifndef _LIGHTPIDCONTROL_H_
#define _LIGHTPIDCONTROL_H_

#include "ecrobot_interface.h"
#include "Calibration.h"

//���C���g���[�X��PID����p�W��
static float Kp = 2.136;//0.436;//0.436;				//P����p
static float Ki = 0.8;//1.31;//0;//1.31;					//I����p
static float Kd = 0.080;//0.27;//0.5;//0.074;//0.011;				//D����p

//PID����p�΍��l
static float hensa;					//P����p
static float i_hensa = 0;			//I����p
static float d_hensa = 0;			//D����p
static float bf_hensa = 0;

static int graybase = 0;

extern int RA_linetrace_PID(int forward_speed);
extern void RA_hensareset();
extern int RA_linetrace();
extern int online();

extern void changeGray();

#endif

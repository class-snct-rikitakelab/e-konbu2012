#ifndef _LIGHTPIDCONTROL_H_
#define _LIGHTPIDCONTROL_H_

#include "ecrobot_interface.h"
#include "Calibration.h"

//���C���g���[�X��PID����p�W��
//static float Kp =0.636;//1.836;//������"3"0.536;//1.53;//�ȑO�܂ł�1.8;//0.536;//1.836;//0.436;//0.436;				//P����p
//static float Ki = 0.5;//������"3"0.6;//1.2;//�ȑO�܂ł�0.6;//1.6;//1.31;//0;//1.31;					//I����p
//static float Kd = 0.09;//������"3"0.06;//0.006;//0.3;//�ȑO�܂ł�0.006;//0.27;//0.5;//0.074;//0.011;				//D����p

static float Kp =0.836;//1.836;//������"3"0.536;//1.53;//�ȑO�܂ł�1.8;//0.536;//1.836;//0.436;//0.436;				//P����p
static float Ki = 0.5;//������"3"0.6;//1.2;//�ȑO�܂ł�0.6;//1.6;//1.31;//0;//1.31;					//I����p
static float Kd = 0.09;//������"3"0.06;//0.006;//0.3;//�ȑO�܂ł�0.006;//0.27;//0.5;//0.074;//0.011;				//D����p

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
#include "MainRunningInCourse.h"

//ライントレース時PID制御用係数
static float Kp = 0.446;//0.436;//0.436;				//P制御用
static float Ki = 1.31;//0;//1.31;					//I制御用
static float Kd = 0.005;//0.074;//0.011;				//D制御用


extern void RA_linetrace_PID(int forward_speed);
#include "MainRunningInCourse.h"

//���C���g���[�X��PID����p�W��
static float Kp = 0.446;//0.436;//0.436;				//P����p
static float Ki = 1.31;//0;//1.31;					//I����p
static float Kd = 0.005;//0.074;//0.011;				//D����p


extern void RA_linetrace_PID(int forward_speed);
//#include "MainRunningInCourse.h"
#include "ecrobot_interface.h"
#include "Calibration.h"

//���C���g���[�X��PID����p�W��
static float Kp = 0.446;//0.436;//0.436;				//P����p
static float Ki = 1.31;//0;//1.31;					//I����p
static float Kd = 0.005;//0.074;//0.011;				//D����p

//PID����p�΍��l
static float hensa;					//P����p
static float i_hensa = 0;			//I����p
static float d_hensa = 0;			//D����p
static float bf_hensa = 0;

extern int RA_linetrace_PID(int forward_speed);
extern void RA_hensareset();

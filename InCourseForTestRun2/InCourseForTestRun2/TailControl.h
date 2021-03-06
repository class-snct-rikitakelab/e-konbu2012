#ifndef _TAILCONTROL_H_
#define _TAILCONTROL_H_

#include "kernel.h"
#include "kernel_id.h"
#include "ecrobot_interface.h"
#include "balancer.h"
#include "math.h"

#define ANGLEOFDOWN		95	//Kös
#define ANGLEOFZERO		0
#define ANGLEOFUP		6	//Kösgp
#define ANGLEOFLOOKUP	56	//bNAbvQ[gÊß
#define ANGLEOFSTAND	100	//¼§óÔ
#define ANGLEOFPUSH		120	//sÌµËã°

#define TAIL_ANGLE_COUNT 5
/*
//KöÌóÔ
typedef enum{
	RN_TAILDOWN,				//Kö~º
	RN_TAILUP,					//Köã¸
	RN_TAILLOOKUP,
	RN_TAILSTAND,
	RN_TAILPUSH,
} RN_TAILMODE;
*/
//KöPI§äpW
static float t_Kp = 5.85;			//P§äp
static float t_Ki = 0;				//I§äp

static int result_angle = 0;
static int target_angle = ANGLEOFZERO;

extern void TailControl();
extern void TargetTailAngleControl();
extern void TailAngleChange(int angle);

//RN_TAILMODE controltailmode = ANGLEOFZERO;

#endif

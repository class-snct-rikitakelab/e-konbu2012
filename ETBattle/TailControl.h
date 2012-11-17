#ifndef _TAILCONTROL_H_
#define _TAILCONTROL_H_

#include "kernel.h"
#include "kernel_id.h"
#include "ecrobot_interface.h"
#include "balancer.h"
#include "math.h"

#define ANGLEOFDOWN		105	//�K�����s
#define ANGLEOFZERO		0
#define ANGLEOFUP		6	//�K���s�g�p

#define TAIL_ANGLE_COUNT 5

//�K��PI����p�W��
static float t_Kp = 8.85;			//P����p

static int result_angle = 0;
static int target_angle = ANGLEOFZERO;

extern void TailControl();
extern void TargetTailAngleControl();
extern void TailAngleChange(int angle);

#endif

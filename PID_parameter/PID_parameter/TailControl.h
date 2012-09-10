//#include "MainRunningInCourse.h"
#ifndef _TAILCONTROL_H_
#define _TAILCONTROL_H_

#include "kernel.h"
#include "kernel_id.h"
#include "ecrobot_interface.h"
#include "balancer.h"
#include "math.h"

#define ANGLEOFDOWN		90	//�K�����s
#define ANGLEOFZERO		0
#define ANGLEOFUP		6	//�K���s�g�p
#define ANGLEOFLOOKUP	56	//���b�N�A�b�v�Q�[�g�ʉ�
#define ANGLEOFSTAND	100	//�������
#define ANGLEOFPUSH		120	//���s�̒��ˏグ

#define TAIL_ANGLE_COUNT 5

//�K���̏��
typedef enum{
	RN_TAILDOWN,				//�K���~��
	RN_TAILUP,					//�K���㏸
	RN_TAILLOOKUP,
	RN_TAILSTAND,
	RN_TAILPUSH,
} RN_TAILMODE;

//�K��PI����p�W��
static float t_Kp = 8.85;			//P����p
static float t_Ki = 0;				//I����p

static int result_angle = 0;

RN_TAILMODE controltailmode = ANGLEOFZERO;

extern void TailControl(void);
extern void TargetTailAngleControl(void);
extern void TailModeChange(RN_TAILMODE tail_mode);

#endif

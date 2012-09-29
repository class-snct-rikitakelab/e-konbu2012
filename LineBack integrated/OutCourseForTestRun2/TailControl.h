#ifndef _TAILCONTROL_H_
#define _TAILCONTROL_H_

#include "kernel.h"
#include "kernel_id.h"
#include "ecrobot_interface.h"
#include "balancer.h"
#include "math.h"

#define ANGLEOFDOWN		95	//�K�����s
#define ANGLEOFZERO		0
#define ANGLEOFUP		6	//�K���s�g�p
#define ANGLEOFLOOKUP	56	//���b�N�A�b�v�Q�[�g�ʉ�
#define ANGLEOFSTAND	103	//�������
#define ANGLEOFPUSH		110	//���s�̒��ˏグ
#define ANGLEOFSLOPEUP	80	//�⓹����
#define	ANGLEOFSLOPEDOWN	110	//�⓹���

#define TAIL_ANGLE_COUNT 1 //3
/*
//�K���̏��
typedef enum{
	RN_TAILDOWN,				//�K���~��
	RN_TAILUP,					//�K���㏸
	RN_TAILLOOKUP,
	RN_TAILSTAND,
	RN_TAILPUSH,
} RN_TAILMODE;
*/
//�K��PI����p�W��
static float t_Kp = 5.85;			//P����p
static float t_Ki = 0;				//I����p

static int result_angle = 0;
static int target_angle = ANGLEOFZERO;

extern void TailControl();
extern void TargetTailAngleControl();
extern void TailAngleChange(int angle);

//RN_TAILMODE controltailmode = ANGLEOFZERO;

#endif

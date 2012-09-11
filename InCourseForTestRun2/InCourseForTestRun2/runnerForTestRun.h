
#ifndef _RUNNER_H_
#define _RUNNER_H_

#include "kernel.h"
#include "kernel_id.h"
#include "ecrobot_interface.h"
#include "balancer.h"

//�W���C���U���l
#define PM_GYRO 65
/* ���s��� */
typedef enum{
	START,
	UP_SLOPE,
	DOWN_SLOPE,
	FST_CORNER,
	FST_STRAIGHT,
	SND_CORNER,
	SND_STRAIGHT,
	TRD_CORNER,
	TRD_STRAIGHT,
	FIN_APPROACH,
	B_FST_STRAIGHT,
	B_FST_CORNER,
	B_SND_STRAIGHT,
	B_SND_CORNER,
	B_TRD_STRAIGHT,
	B_TRD_CORNER,
	LOOKUP
} IN_SECTION;

//�ڕW�ȗ����a
static float trgt_R = 0;

//�}�[�J�[�t���O�@0: OFF, 1: ON
unsigned char m_flg = 0;
static unsigned int LV_buf = 0;		/* Light Value buffer */





extern S8 trgt_speed = 100;

extern S8 RA_directionCtrl_PID(float target);//�ԑ̊p�x���ڕW�p�x�ɂȂ�悤�ɐ���ʂ��v�Z
extern S8 RA_curvatureCtrl_PID(float target);//�����œn���ꂽ�ڕW�ȗ��ɋ߂Â��悤�ɐ���ʂ��v�Z

void setLocation(void);
extern void setSection_in(void);
extern void setSection_out(void);

unsigned char MKTrigger(void);
signed char LVTrigger(void);


void changeSection(float *buf_x, float *buf_y, float *buf_l, float *buf_th);

//�Ⴂ���s��


extern float getTargetR(); //���݋�Ԃ̖ڕW�ȗ����擾

#endif

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
	MARKER,
	STEPS,
	TURN90D
} OUT_SECTION;

static OUT_SECTION crt_sect = START;

//�ڕW�ȗ����a
static float trgt_R = 0;

//�}�[�J�[�t���O�@0: OFF, 1: ON
unsigned char m_flg = 0;
static unsigned int LV_buf = 0;		/* Light Value buffer */


static float buf_x = 0.0, buf_y = 0.0, buf_l = 0.0, buf_th = 0.0; //��Ԑ���p�o�b�t�@


extern S8 trgt_speed = 80;

extern S8 RA_directionCtrl_PID(float target);//�ԑ̊p�x���ڕW�p�x�ɂȂ�悤�ɐ���ʂ��v�Z
extern S8 RA_curvatureCtrl_PID(float target);//�����œn���ꂽ�ڕW�ȗ��ɋ߂Â��悤�ɐ���ʂ��v�Z

void setLocation(void);
extern void setSection_out(void);

unsigned char MKTrigger(void);
signed char LVTrigger(void);


void changeSection(float *buf_x, float *buf_y, float *buf_l, float *buf_th);

//�Ⴂ���s��


extern float getTargetR(); //���݋�Ԃ̖ڕW�ȗ����擾
extern S8 getTargSpeed();
extern void bufClear(); //��Ԑ���p�o�b�t�@�̃N���A
#endif

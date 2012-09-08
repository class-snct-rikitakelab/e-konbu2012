
#ifndef _RUNNER_H_
#define _RUNNER_H_

#include "kernel.h"
#include "kernel_id.h"
#include "ecrobot_interface.h"
#include "balancer.h"

//�W���C���U���l
#define PM_GYRO 65


static unsigned int GYRO_OFFSET;


//���x�J�E���^�̏���l
static int SPEED_COUNT = 60;

//���x���߃J�E���^�i�J�E���^������ɒB���閈�ɑ��x��1�㏸
static int speed_counter = 0;






//�ڕW�ȗ����a
static float trgt_R = 0;

//�}�[�J�[�t���O�@0: OFF, 1: ON
unsigned char m_flg = 0;
static unsigned int LV_buf = 0;		/* Light Value buffer */



/* �C���R�[�X���s��� */
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
	LOOKUP
} SECT_IN;

/* �A�E�g�R�[�X���s��� */
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
	90DTURN
} SECT_OUT;



extern S8 RA_directionCtrl_PID(float target);�@//�ԑ̊p�x���ڕW�p�x�ɂȂ�悤�ɐ���ʂ��v�Z
extern S8 RA_curvatureCtrl_PID(float target);�@//�����œn���ꂽ�ڕW�ȗ��ɋ߂Â��悤�ɐ���ʂ��v�Z

void setLocation(void);
void setSection_in(void);
void setSection_out(void);
void cngSection(float *buf_x, float *buf_y, float *buf_l, float *buf_th);
unsigned char MKTrigger(void);
signed char LVTrigger(void);

extern float getTartgetR(); //���݋�Ԃ̖ڕW�ȗ����擾

#endif

#ifndef _RUNNER_H_
#define _RUNNER_H_

#include "kernel.h"
#include "kernel_id.h"
#include "ecrobot_interface.h"
#include "balancer.h"
#include "TailControl.h"

//�W���C���U���l
#define PM_GYRO 65
#define TARGET_SPEED 65�@�@//���s�J�n����forward�l

/* ���s��� */
typedef enum{
	START,
	UP_SLOPE,
	SLOPE_SLOW_DOWN,
	DOWN_SLOPE,
	MINI_STRARIGHT,
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

/*______________base forward speed*/
static S8 trgt_speed = TARGET_SPEED;
/*_____________________*/
static  S8 trgt_theta = -1;
extern  IN_SECTION crt_sect = START;

//�ڕW�ԑ̊p�x

//�}�[�J�[�t���O�@0: OFF, 1: ON
unsigned char m_flg = 0;
static unsigned int LV_buf = 0;		/* Light Value buffer */

static float buf_x = 0.0, buf_y = 0.0, buf_l = 0.0, buf_th = 0.0; //��Ԑ���p�o�b�t�@

static float RKp=1.2,RKi=0,RKd=0.03;

extern int RA_directionCtrl_PID(float target);//�ԑ̊p�x���ڕW�p�x�ɂȂ�悤�ɐ���ʂ��v�Z
extern int RA_curvatureCtrl_PID(float target);//�����œn���ꂽ�ڕW�ȗ��ɋ߂Â��悤�ɐ���ʂ��v�Z

void setLocation(void);
extern void setSection_in(void);
extern void setSection_out(void);

unsigned char MKTrigger(void);
signed char LVTrigger(void);


void changeSection(float *buf_x, float *buf_y, float *buf_l, float *buf_th);


extern S8 getTargetTheta();
extern float getTargetR(); //���݋�Ԃ̖ڕW�ȗ����擾
extern S8 getTargSpeed();
extern void bufClear(); //��Ԑ���p�o�b�t�@�̃N���A
#endif

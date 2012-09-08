#ifndef _SELFLOCATION_H_
#define _SELFLOCATION_H_

#include "math.h"
#include "ecrobot_interface.h"

/* �ԗ֔��a�A���s�̕�*/
#define W_RADIUS	4.1	//[mm]
#define W_DIST		16.2//[mm]


/* ���Ȉʒu����p�ϐ� */
float d_theta_r;			//1�X�e�b�v������̉E�ԗ։�]�p�x
float d_theta_l;			//1�X�e�b�v������̍��ԗ։�]�p�x

static float d_theta_r_t = 0;		//�O��̃X�e�b�v�̉E�ԗ։�]�p�x
static float d_theta_l_t = 0;		//�O��̃X�e�b�v�̍��ԗ։�]�p�x

static double omega_r;			//�E�ԗւ̉�]�p���x
static double omega_l;			//���ԗւ̉�]�p���x

float v_r;				//�E�ԗւ̉�]���x
float v_l;				//���ԗւ̉�]���x

float v;					//���s�̂̑��s���x
float omega;				//���s�̂̉�]�p���x

static float x_r = 0;		//�ԑ̂�X���W
static float y_r = 0;		//�ԑ̂�Y���W

static float x_r_zero = 0;	//X���W�����l
static float y_r_zero = 0;	//Y���W�����l
static float theta_R_zero = 0;	//�ԑ̊p�x�����l

static float dist = 0;		//�ړ�����
static float theta = 0;		//�ԑ̂̊p�x
static float R = 0;			//�ȗ����a

extern void selflocation(void);

#endif

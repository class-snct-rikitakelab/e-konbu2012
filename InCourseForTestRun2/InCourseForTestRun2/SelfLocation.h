//#include "MainRunningInCourse.h"

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
static float theta_R = 0;	//�ԑ̂̊p�x

static float x_r_zero = 0;	//X���W�����l
static float y_r_zero = 0;	//Y���W�����l
static float theta_R_zero = 0;	//�ԑ̊p�x�����l

extern void selflocation(void);

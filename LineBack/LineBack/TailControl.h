#ifndef __TailControl
#define __TailControl

#include "config.h"

//�K���̏��
typedef enum{
	TAIL_DOWN,		//�K���~��
	TAIL_UP,		//�K���㏸
} TailState;

typedef struct{
	float Tail_Kp;	//���W��
	float Tail_Ki;	//�ϕ��萔
	float Tail_Kd;	//�����W��
	TailState State;	//�K�����
}TailControl;

#define ANGLEOFDOWN		95	//�K�����s
#define ANGLEOFUP		5	//�K���s�g�p
#define ANGLEOFLOOKUP	56	//���b�N�A�b�v�Q�[�g�ʉ�
#define ANGLEOFSTAND	100	//�������
#define ANGLEOFPUSH		120	//���s�̒��ˏグ

extern void TailControl_init(TailControl * this_TailControl);
extern void TailControl_PIDTailControl(TailControl * this_TailControl);
extern void TailControl_TailStateChange(TailControl * this_TailControl,TailState state);

#endif

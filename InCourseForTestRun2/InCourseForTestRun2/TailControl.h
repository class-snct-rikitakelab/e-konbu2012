#include "MainRunningInCourse.h"

#define ANGLEOFDOWN		95	//�K�����s
#define ANGLEOFUP		5	//�K���s�g�p
#define ANGLEOFLOOKUP	56	//���b�N�A�b�v�Q�[�g�ʉ�
#define ANGLEOFSTAND	100	//�������
#define ANGLEOFPUSH		120	//���s�̒��ˏグ

//�K���̏��
typedef enum{
	RN_TAILDOWN,				//�K���~��
	RN_TAILUP,					//�K���㏸
} RN_TAILMODE;

//�K��PI����p�W��
static float t_Kp = 1.85;			//P����p
static float t_Ki = 0;				//I����p
